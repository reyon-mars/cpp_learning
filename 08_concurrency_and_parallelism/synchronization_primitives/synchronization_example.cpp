#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <string_view>
#include <atomic>
#include <format>
#include <ranges>
#include <latch>
#include <semaphore>
#include <stop_token>
#include <cassert>
#include <functional>

using namespace std::chrono_literals;

namespace {
    std::mutex              mtx;
    std::mutex              mtx2;
    std::timed_mutex        timed_mtx;
    std::shared_mutex       shared_mtx;
    std::mutex              cv_mtx;
    std::condition_variable cv;
    std::recursive_mutex    rec_mtx;
    std::mutex              cout_mtx;
    std::atomic<int>        shared_data{0};
    bool                    ready = false;
}

void safe_print(std::string_view msg) {
    std::lock_guard lock{cout_mtx};
    std::cout << std::format("{}\n", msg);
}

void increment_safe() {
    std::lock_guard lock{mtx};
    for ([[maybe_unused]] int i : std::views::iota(0, 1000))
        ++shared_data;
}

void increment_with_unique_lock() {
    std::unique_lock lock{mtx};
    shared_data += 500;
    lock.unlock();
    safe_print("unique_lock released early");
}

void try_increment() {
    if (std::unique_lock lock{mtx, std::try_to_lock}; lock.owns_lock()) {
        shared_data += 10;
        safe_print("try_lock succeeded");
    } else {
        safe_print("try_lock failed");
    }
}

void safe_dual_lock() {
    std::scoped_lock lock{mtx, mtx2};
    shared_data += 20;
}

void deadlock_safe_demo() {
    std::scoped_lock lock{mtx, mtx2};
    shared_data += 30;
    safe_print("scoped_lock (deadlock-safe) used");
}

void timed_try_increment() {
    if (timed_mtx.try_lock_for(10ms)) {
        shared_data += 15;
        safe_print("timed_mutex lock succeeded");
        timed_mtx.unlock();
    } else {
        safe_print("timed_mutex lock timed out");
    }
}

void unique_lock_timed() {
    std::unique_lock lock{timed_mtx, std::defer_lock};
    if (lock.try_lock_for(5ms)) {
        shared_data += 25;
        safe_print("unique_lock timed succeeded");
    } else {
        safe_print("unique_lock timed failed");
    }
}

void batch_increment(int times) {
    for ([[maybe_unused]] int i : std::views::iota(0, times)) {
        std::lock_guard lock{mtx};
        ++shared_data;
    }
}

void reader_task() {
    std::shared_lock lock{shared_mtx};
    safe_print(std::format("Reader: shared_data={}", shared_data.load()));
}

void writer_task() {
    std::unique_lock lock{shared_mtx};
    shared_data += 100;
    safe_print("Writer: updated shared_data");
}

void wait_task() {
    std::unique_lock lock{cv_mtx};
    cv.wait(lock, [] { return ready; });
    safe_print("cv: condition satisfied");
}

void notify_task() {
    {
        std::lock_guard lock{cv_mtx};
        ready = true;
    }
    cv.notify_one();
    safe_print("cv: notified");
}

void recursive_function(int depth) {
    std::lock_guard lock{rec_mtx};
    safe_print(std::format("recursive depth={}", depth));
    if (depth > 0) recursive_function(depth - 1);
}

class Spinlock {
    std::atomic_flag flag_{};
public:
    void lock() noexcept {
        while (flag_.test_and_set(std::memory_order_acquire))
            flag_.wait(true, std::memory_order_relaxed);
    }
    void unlock() noexcept {
        flag_.clear(std::memory_order_release);
        flag_.notify_one();
    }
    [[nodiscard]] bool try_lock() noexcept {
        return !flag_.test_and_set(std::memory_order_acquire);
    }
};

static_assert(std::is_nothrow_default_constructible_v<Spinlock>);

void spinlock_demo() {
    static Spinlock spin;
    std::lock_guard lock{spin};
    shared_data.fetch_add(1, std::memory_order_relaxed);
}

void once_demo() {
    static std::once_flag flag;
    std::call_once(flag, [] { safe_print("call_once: initialised exactly once"); });
}

int main() {
    {
        std::vector<std::jthread> threads;
        threads.reserve(15);

        for ([[maybe_unused]] int i : std::views::iota(0, 5))
            threads.emplace_back(increment_safe);

        threads.emplace_back(increment_with_unique_lock);
        threads.emplace_back(try_increment);
        threads.emplace_back(safe_dual_lock);
        threads.emplace_back(timed_try_increment);
        threads.emplace_back(unique_lock_timed);
        threads.emplace_back(batch_increment, 50);
        threads.emplace_back(reader_task);
        threads.emplace_back(writer_task);
        threads.emplace_back(wait_task);
        threads.emplace_back(notify_task);
        threads.emplace_back(deadlock_safe_demo);
        threads.emplace_back([] { safe_print("thread-safe print demo"); });
    }

    std::cout << "\n=== Recursive mutex ===\n";
    recursive_function(3);

    std::cout << "\n=== Spinlock ===\n";
    {
        std::vector<std::jthread> workers;
        workers.reserve(8);
        for ([[maybe_unused]] int i : std::views::iota(0, 8))
            workers.emplace_back(spinlock_demo);
    }
    safe_print(std::format("After spinlock workers: +8 increments applied"));

    std::cout << "\n=== std::call_once ===\n";
    {
        std::vector<std::jthread> callers;
        callers.reserve(4);
        for ([[maybe_unused]] int i : std::views::iota(0, 4))
            callers.emplace_back(once_demo);
    }

    std::cout << "\n=== Semaphore rate-limiter ===\n";
    {
        std::counting_semaphore<3> sem(3);
        std::atomic<int>           active{0};
        std::latch                 done(6);

        std::vector<std::jthread> workers;
        workers.reserve(6);
        for (int i : std::views::iota(0, 6)) {
            workers.emplace_back([&, i] {
                sem.acquire();
                const int n = active.fetch_add(1, std::memory_order_acq_rel) + 1;
                safe_print(std::format("Semaphore worker {} active (concurrent={})", i, n));
                std::this_thread::sleep_for(10ms);
                active.fetch_sub(1, std::memory_order_acq_rel);
                sem.release();
                done.count_down();
            });
        }
        done.wait();
    }

    std::cout << "\n=== jthread cooperative stop ===\n";
    {
        std::atomic<int> ticks{0};
        std::counting_semaphore<1> stopped(0);
        std::jthread worker([&](std::stop_token st) {
            while (!st.stop_requested()) {
                ticks.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::sleep_for(8ms);
            }
            stopped.release();
        });
        std::this_thread::sleep_for(50ms);
        worker.request_stop();
        stopped.acquire();
        safe_print(std::format("jthread ticked {} times before stop", ticks.load()));
        assert(ticks.load() > 0);
    }

    std::cout << std::format("\nFinal shared_data={}\n", shared_data.load());
    return 0;
}
