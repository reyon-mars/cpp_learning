// Synchronization Primitives Exercise
// mutex, lock_guard, unique_lock

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>

std::mutex mtx;
std::mutex mtx2;
int shared_data = 0;

// ---------------- ORIGINAL FUNCTIONS ----------------

void increment_safe() {
    std::lock_guard<std::mutex> lock(mtx);
    for (int i = 0; i < 1000; ++i) {
        shared_data++;
    }
}

void increment_with_unique_lock() {
    std::unique_lock<std::mutex> lock(mtx);
    shared_data += 500;
    lock.unlock();  // Can unlock early if needed
    std::cout << "Lock released\n";
}

void try_increment() {
    if (mtx.try_lock()) {
        shared_data += 10;
        std::cout << "try_lock succeeded\n";
        mtx.unlock();
    } else {
        std::cout << "try_lock failed\n";
    }
}

// Demonstrates locking multiple mutexes safely
void safe_dual_lock() {
    std::scoped_lock lock(mtx, mtx2);
    shared_data += 20;
}

// ---------------- SMALL ADDITIONS ----------------

// Manual lock/unlock example
void manual_increment() {
    mtx.lock();
    shared_data += 5;
    std::cout << "manual lock used\n";
    mtx.unlock();
}

// Timed attempt using try_lock with delay simulation
void delayed_try_increment() {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    if (mtx.try_lock()) {
        shared_data += 15;
        std::cout << "delayed try_lock succeeded\n";
        mtx.unlock();
    } else {
        std::cout << "delayed try_lock failed\n";
    }
}

// Thread-safe printing
std::mutex cout_mtx;
void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mtx);
    std::cout << msg << "\n";
}

// 🔹 NEW: retry mechanism using try_lock
void retry_increment() {
    int attempts = 3;
    while (attempts--) {
        if (mtx.try_lock()) {
            shared_data += 7;
            std::cout << "retry_increment succeeded\n";
            mtx.unlock();
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    std::cout << "retry_increment failed after retries\n";
}

// 🔹 NEW: unique_lock with defer_lock
void deferred_lock_example() {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    if (lock.try_lock()) {
        shared_data += 12;
        std::cout << "deferred lock acquired\n";
    }
}

// 🔹 NEW: batch safe increment
void batch_increment(int times) {
    for (int i = 0; i < times; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        shared_data++;
    }
}

// ---------------- MAIN ----------------

int main() {

    std::vector<std::thread> threads;

    // lock_guard threads
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(increment_safe);
    }

    // unique_lock thread
    threads.emplace_back(increment_with_unique_lock);

    // try_lock thread
    threads.emplace_back(try_increment);

    // scoped_lock thread
    threads.emplace_back(safe_dual_lock);

    // ---------------- ADDED THREADS ----------------
    threads.emplace_back(manual_increment);
    threads.emplace_back(delayed_try_increment);

    threads.emplace_back([]() {
        safe_print("Thread-safe print demo");
    });

    // 🔹 NEW threads
    threads.emplace_back(retry_increment);
    threads.emplace_back(deferred_lock_example);
    threads.emplace_back(batch_increment, 50);

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final value: " << shared_data << "\n";

    return 0;
}
