#include <iostream>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <string_view>
#include <atomic>

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

    std::atomic<int> shared_data{0};
    bool             ready = false;
}

void safe_print(std::string_view msg) {
    std::lock_guard lock{cout_mtx};
    std::cout << msg << "\n";
}

void increment_safe() {
    std::lock_guard lock{mtx};
    for (int i = 0; i < 1000; ++i) ++shared_data;
}

void increment_with_unique_lock() {
    std::unique_lock lock{mtx};
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
    for (int i = 0; i < times; ++i) {
        std::lock_guard lock{mtx};
        ++shared_data;
    }
}

void reader_task() {
    std::shared_lock lock{shared_mtx};
    safe_print("Reader: shared_data=" + std::to_string(shared_data.load()));
}

void writer_task() {
    std::unique_lock lock{shared_mtx};
    shared_data += 100;
    safe_print("Writer: updated shared_data");
}

void wait_task() {
    std::unique_lock lock{cv_mtx};
    cv.wait(lock, []{ return ready; });
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
    safe_print("recursive depth=" + std::to_string(depth));
    if (depth > 0) recursive_function(depth - 1);
}

int main() {
    std::vector<std::thread> threads;
    threads.reserve(15);

    for (int i = 0; i < 5; ++i) threads.emplace_back(increment_safe);
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
    threads.emplace_back([]{ safe_print("thread-safe print demo"); });

    for (auto& t : threads) t.join();

    std::cout << "\n=== Recursive mutex ===\n";
    recursive_function(3);

    std::cout << "\nFinal shared_data=" << shared_data.load() << "\n";
    return 0;
}
