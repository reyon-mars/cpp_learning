#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <shared_mutex>
#include <condition_variable>
#include <format>
#include <concepts>
#include <atomic>
#include <latch>
#include <barrier>
#include <semaphore>
#include <stop_token>
#include <cassert>
#include <functional>
#include <span>
#include <ranges>

namespace {

std::mutex              mtx;
std::mutex              mtx2;
std::mutex              cout_mtx;
std::shared_mutex       shared_mtx;
std::condition_variable cv;
std::recursive_mutex    rec_mtx;

int  shared_data = 0;
bool cv_ready    = false;

void safe_print(std::string_view msg) {
    std::lock_guard lock(cout_mtx);
    std::cout << std::format("{}\n", msg);
}

void increment_safe() {
    std::lock_guard lock(mtx);
    for (int i = 0; i < 1000; ++i)
        ++shared_data;
}

void increment_with_unique_lock() {
    std::unique_lock lock(mtx);
    shared_data += 500;
    lock.unlock();
    safe_print("unique_lock released early");
}

void try_increment() {
    if (mtx.try_lock()) {
        shared_data += 10;
        safe_print("try_lock succeeded");
        mtx.unlock();
    } else {
        safe_print("try_lock failed");
    }
}

void safe_dual_lock() {
    std::scoped_lock lock(mtx, mtx2);
    shared_data += 20;
}

void manual_increment() {
    std::lock_guard lock(mtx);
    shared_data += 5;
    safe_print("manual lock used");
}

void delayed_try_increment() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    if (mtx.try_lock()) {
        shared_data += 15;
        safe_print("delayed try_lock succeeded");
        mtx.unlock();
    } else {
        safe_print("delayed try_lock failed");
    }
}

void retry_increment(int attempts = 3) {
    while (attempts-- > 0) {
        if (std::unique_lock lock(mtx, std::try_to_lock); lock.owns_lock()) {
            shared_data += 7;
            safe_print("retry_increment succeeded");
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    safe_print("retry_increment failed after retries");
}

void deferred_lock_example() {
    std::unique_lock lock(mtx, std::defer_lock);
    if (lock.try_lock()) {
        shared_data += 12;
        safe_print("deferred lock acquired");
    }
}

void batch_increment(int times) {
    for (int i = 0; i < times; ++i) {
        std::lock_guard lock(mtx);
        ++shared_data;
    }
}

void shared_reader() {
    std::shared_lock lock(shared_mtx);
    safe_print(std::format("Reader sees: {}", shared_data));
}

void shared_writer() {
    std::unique_lock lock(shared_mtx);
    shared_data += 100;
    safe_print("Writer updated shared_data");
}

void waiting_thread() {
    std::unique_lock lock(mtx);
    cv.wait(lock, [] { return cv_ready; });
    safe_print("Condition variable triggered");
}

void notifier_thread() {
    {
        std::lock_guard lock(mtx);
        cv_ready = true;
    }
    cv.notify_one();
}

void recursive_demo(int depth) {
    std::lock_guard lock(rec_mtx);
    safe_print(std::format("Recursive depth: {}", depth));
    if (depth > 0)
        recursive_demo(depth - 1);
}

void adopt_lock_demo() {
    std::lock(mtx, mtx2);
    std::lock_guard lock1(mtx,  std::adopt_lock);
    std::lock_guard lock2(mtx2, std::adopt_lock);
    shared_data += 30;
    safe_print("adopt_lock demo executed");
}

class BoundedQueue {
    std::vector<int>        buffer_;
    const std::size_t       capacity_;
    mutable std::mutex      mtx_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
    std::atomic<bool>       closed_{false};

public:
    explicit BoundedQueue(std::size_t cap) : capacity_(cap) {
        buffer_.reserve(cap);
    }

    bool push(int val) {
        std::unique_lock lock(mtx_);
        not_full_.wait(lock, [&] {
            return buffer_.size() < capacity_ || closed_.load(std::memory_order_acquire);
        });
        if (closed_.load(std::memory_order_acquire)) return false;
        buffer_.push_back(val);
        not_empty_.notify_one();
        return true;
    }

    bool pop(int& out) {
        std::unique_lock lock(mtx_);
        not_empty_.wait(lock, [&] {
            return !buffer_.empty() || closed_.load(std::memory_order_acquire);
        });
        if (buffer_.empty()) return false;
        out = buffer_.back();
        buffer_.pop_back();
        not_full_.notify_one();
        return true;
    }

    void close() {
        closed_.store(true, std::memory_order_release);
        not_full_.notify_all();
        not_empty_.notify_all();
    }

    [[nodiscard]] std::size_t size() const {
        std::shared_lock lock(mtx_);
        return buffer_.size();
    }
};

template <std::invocable<> Fn>
void run_guarded(Fn&& fn, std::string_view label) {
    try {
        std::invoke(std::forward<Fn>(fn));
    } catch (const std::exception& e) {
        safe_print(std::format("[{}] exception: {}", label, e.what()));
    }
}

} // namespace

int main() {
    std::vector<std::jthread> threads;
    threads.reserve(20);

    auto spawn = [&](auto fn, auto... args) {
        threads.emplace_back(fn, args...);
    };

    for (int i = 0; i < 5; ++i) spawn(increment_safe);
    spawn(increment_with_unique_lock);
    spawn(try_increment);
    spawn(safe_dual_lock);
    spawn(manual_increment);
    spawn(delayed_try_increment);
    spawn([] { safe_print("Thread-safe print demo"); });
    spawn(retry_increment, 3);
    spawn(deferred_lock_example);
    spawn(batch_increment, 50);
    spawn(shared_reader);
    spawn(shared_writer);
    spawn(waiting_thread);
    spawn(notifier_thread);
    spawn(adopt_lock_demo);

    threads.clear();

    std::cout << "\n--- Recursive Mutex Demo ---\n";
    recursive_demo(3);

    std::cout << std::format("\nshared_data after core demos: {}\n", shared_data);

    std::cout << "\n--- BoundedQueue producer/consumer ---\n";
    {
        constexpr int item_count = 10;
        BoundedQueue  queue(4);
        std::latch    done(2);

        std::jthread producer([&] {
            for (int i = 0; i < item_count; ++i) {
                if (!queue.push(i)) break;
                safe_print(std::format("Pushed {}", i));
            }
            queue.close();
            done.count_down();
        });

        std::jthread consumer([&] {
            int val;
            while (queue.pop(val))
                safe_print(std::format("Popped {}", val));
            done.count_down();
        });

        done.wait();
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

    std::cout << "\n--- jthread cooperative cancellation ---\n";
    {
        std::mutex              log_mtx;
        std::vector<int>        log;
        std::counting_semaphore done_sem(0);

        std::jthread worker([&](std::stop_token st) {
            int tick = 0;
            while (!st.stop_requested()) {
                {
                    std::lock_guard lock(log_mtx);
                    log.push_back(tick++);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(8));
            }
            done_sem.release();
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        worker.request_stop();
        done_sem.acquire();

        std::lock_guard lock(log_mtx);
        safe_print(std::format("Worker ticked {} times before stop", log.size()));
        assert(!log.empty());
    }

    std::cout << "\n--- std::barrier phased sync ---\n";
    {
        constexpr int           n_threads = 4;
        constexpr int           n_phases  = 3;
        std::atomic<int>        phase_sum{0};
        std::vector<std::jthread> workers;
        workers.reserve(n_threads);

        std::barrier sync(n_threads, [&] noexcept {
            safe_print(std::format("Barrier phase complete, sum={}", phase_sum.load()));
            phase_sum.store(0, std::memory_order_relaxed);
        });

        for (int id : std::views::iota(0, n_threads)) {
            workers.emplace_back([&, id] {
                for (int p : std::views::iota(0, n_phases)) {
                    phase_sum.fetch_add(id + 1, std::memory_order_relaxed);
                    safe_print(std::format("Thread {} done phase {}", id, p));
                    sync.arrive_and_wait();
                }
            });
        }
    }

    std::cout << "\n--- stop_callback demo ---\n";
    {
        std::atomic<bool> cleanup_ran{false};
        std::counting_semaphore<1> ready(0);

        std::jthread worker([&](std::stop_token st) {
            std::stop_callback cb(st, [&] noexcept {
                cleanup_ran.store(true, std::memory_order_release);
                safe_print("stop_callback: cleanup executed");
            });
            ready.release();
            while (!st.stop_requested())
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
        });

        ready.acquire();
        worker.request_stop();
        worker.join();
        assert(cleanup_ran.load(std::memory_order_acquire));
    }

    std::cout << "\n--- thread_local storage ---\n";
    {
        thread_local int tl_counter = 0;
        std::latch       done(4);
        std::atomic<int> total{0};

        for (int id : std::views::iota(0, 4)) {
            std::jthread([&, id] {
                for ([[maybe_unused]] int i : std::views::iota(0, id + 1))
                    ++tl_counter;
                total.fetch_add(tl_counter, std::memory_order_relaxed);
                safe_print(std::format("Thread {} tl_counter={}", id, tl_counter));
                done.count_down();
            }).detach();
        }
        done.wait();
        assert(total.load() == 1 + 2 + 3 + 4);
        safe_print(std::format("thread_local total={}", total.load()));
    }

    std::cout << std::format("\nFinal shared_data: {}\n", shared_data);
    return 0;
}
