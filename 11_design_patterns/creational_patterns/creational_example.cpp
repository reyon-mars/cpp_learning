#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <vector>
#include <chrono>
#include <numeric>
#include <atomic>
#include <ranges>
#include <format>
#include <condition_variable>
#include <semaphore>
#include <latch>
#include <stop_token>
#include <cassert>
#include <string_view>
#include <functional>
#include <memory>
#include <optional>

std::mutex cout_mtx;

void safe_print(std::string_view msg) {
    std::lock_guard lock(cout_mtx);
    std::cout << std::format("{}\n", msg);
}

class Singleton {
    Singleton() = default;
public:
    Singleton(const Singleton&)            = delete;
    Singleton& operator=(const Singleton&) = delete;

    [[nodiscard]] static Singleton& instance() noexcept {
        static Singleton inst;
        return inst;
    }

    void say_hello() const { safe_print("Singleton says hello"); }

    void print_address() const {
        std::cout << std::format("Singleton address: {}\n",
                                 static_cast<const void*>(this));
    }
};

template <typename T>
class BoundedQueue {
    std::queue<T>           data_;
    mutable std::mutex      mtx_;
    std::condition_variable not_empty_;
    std::condition_variable not_full_;
    const std::size_t       capacity_;
    std::atomic<bool>       closed_{false};

public:
    explicit BoundedQueue(std::size_t cap = 64) : capacity_(cap) {}

    void push(T value) {
        std::unique_lock lock(mtx_);
        not_full_.wait(lock, [&] {
            return data_.size() < capacity_ || closed_.load(std::memory_order_acquire);
        });
        if (closed_.load(std::memory_order_acquire)) return;
        data_.push(std::move(value));
        not_empty_.notify_one();
    }

    [[nodiscard]] bool try_pop(T& out) {
        std::lock_guard lock(mtx_);
        if (data_.empty()) return false;
        out = std::move(data_.front());
        data_.pop();
        not_full_.notify_one();
        return true;
    }

    [[nodiscard]] std::optional<T> pop_wait(std::chrono::milliseconds timeout) {
        std::unique_lock lock(mtx_);
        if (!not_empty_.wait_for(lock, timeout, [&] {
                return !data_.empty() || closed_.load(std::memory_order_acquire);
            }))
            return std::nullopt;
        if (data_.empty()) return std::nullopt;
        T val = std::move(data_.front());
        data_.pop();
        not_full_.notify_one();
        return val;
    }

    void close() {
        closed_.store(true, std::memory_order_release);
        not_empty_.notify_all();
        not_full_.notify_all();
    }

    [[nodiscard]] std::size_t size() const {
        std::lock_guard lock(mtx_);
        return data_.size();
    }

    [[nodiscard]] bool empty() const {
        std::lock_guard lock(mtx_);
        return data_.empty();
    }

    void clear() {
        std::lock_guard lock(mtx_);
        std::queue<T>{}.swap(data_);
        not_full_.notify_all();
    }
};

class SharedData {
    int                  value_{0};
    mutable std::shared_mutex rw_;

public:
    void write(int v) {
        std::unique_lock lock(rw_);
        value_ = v;
        safe_print(std::format("Writer updated value to {}", v));
    }

    void read() const {
        std::shared_lock lock(rw_);
        safe_print(std::format("Reader saw value {}", value_));
    }

    [[nodiscard]] int get() const {
        std::shared_lock lock(rw_);
        return value_;
    }
};

void producer(BoundedQueue<int>& q, int count) {
    for (int i = 1; i <= count; ++i) {
        q.push(i);
        safe_print(std::format("Produced: {}", i));
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

void consumer(BoundedQueue<int>& q, int count) {
    for (int i = 0; i < count; ++i) {
        int value;
        while (!q.try_pop(value))
            std::this_thread::yield();
        safe_print(std::format("Consumed: {}", value));
    }
}

void timed_consumer(BoundedQueue<int>& q, int count) {
    for (int i = 0; i < count; ++i) {
        if (auto val = q.pop_wait(std::chrono::milliseconds(200)))
            safe_print(std::format("Timed consumer got: {}", *val));
        else
            safe_print("Timed consumer: timeout");
    }
}

int main() {
    std::cout << "--- Singleton ---\n";
    Singleton::instance().say_hello();
    Singleton::instance().print_address();

    {
        std::latch ready(2);
        auto singleton_task = [&] {
            ready.count_down();
            Singleton::instance().say_hello();
            safe_print("Singleton accessed from thread");
        };
        std::jthread s1(singleton_task);
        std::jthread s2(singleton_task);
    }

    std::cout << "\n--- Producer-Consumer ---\n";
    {
        BoundedQueue<int> q(8);
        std::jthread p([&] { producer(q, 5); });
        std::jthread c([&] { consumer(q, 5); });
    }

    std::cout << "\n--- Timed Consumer ---\n";
    {
        BoundedQueue<int> q(8);
        std::jthread p([&] { producer(q, 5); });
        std::jthread tc([&] { timed_consumer(q, 5); });
    }

    std::cout << "\n--- Read-Write Lock ---\n";
    {
        SharedData data;
        std::jthread writer([&] { data.write(10); });
        std::jthread r1([&] { data.read(); });
        std::jthread r2([&] { data.read(); });
    }

    std::cout << "\n--- Read-heavy workload ---\n";
    {
        SharedData data;
        std::vector<std::jthread> readers;
        readers.reserve(5);
        for (int i = 0; i < 5; ++i)
            readers.emplace_back([&] { data.read(); });
        std::jthread writer([&] { data.write(20); });
        readers.clear();
        std::cout << std::format("Final value: {}\n", data.get());
    }

    std::cout << "\n--- Worker threads ---\n";
    {
        std::latch done(4);
        for (int id : std::views::iota(1, 5)) {
            std::jthread([id, &done] {
                safe_print(std::format("Worker thread {} running", id));
                done.count_down();
            }).detach();
        }
        done.wait();
    }

    std::cout << "\n--- Atomic counter ---\n";
    {
        std::atomic<int> counter{0};
        auto inc = [&] {
            for ([[maybe_unused]] int i : std::views::iota(0, 1000))
                counter.fetch_add(1, std::memory_order_relaxed);
        };
        std::jthread t1(inc);
        std::jthread t2(inc);
        t1.join();
        t2.join();
        std::cout << std::format("Atomic counter: {}\n", counter.load());
        assert(counter.load() == 2000);
    }

    std::cout << "\n--- Semaphore rate-limiter ---\n";
    {
        std::counting_semaphore<3> sem(3);
        std::atomic<int>           active{0};
        std::latch                 complete(6);
        std::vector<std::jthread>  workers;
        workers.reserve(6);

        for (int i : std::views::iota(0, 6)) {
            workers.emplace_back([&, i] {
                sem.acquire();
                const int n = active.fetch_add(1, std::memory_order_acq_rel) + 1;
                safe_print(std::format("Worker {} active (concurrent: {})", i, n));
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                active.fetch_sub(1, std::memory_order_acq_rel);
                sem.release();
                complete.count_down();
            });
        }
        complete.wait();
    }

    std::cout << "\n--- Queue operations ---\n";
    {
        BoundedQueue<int> q(8);
        for (int v : {1, 2, 3, 4, 5}) q.push(v);
        std::cout << std::format("Queue size: {}\n", q.size());
        q.clear();
        std::cout << std::format("Empty after clear: {}\n", q.empty() ? "Yes" : "No");
        assert(q.empty());
    }

    const std::vector<int> nums = {1, 2, 3, 4, 5};
    std::cout << std::format("Accumulated sum: {}\n",
                             std::reduce(nums.begin(), nums.end(), 0));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
