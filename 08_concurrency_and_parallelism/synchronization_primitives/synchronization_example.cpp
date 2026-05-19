// Synchronization Primitives Exercise
// mutex, lock_guard, unique_lock

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <shared_mutex>   // 🔹 ADDED
#include <condition_variable> // 🔹 ADDED

std::mutex mtx;
std::mutex mtx2;
std::timed_mutex timed_mtx;   // 🔹 NEW
std::shared_mutex shared_mtx; // 🔹 NEW
std::condition_variable cv;   // 🔹 NEW

int shared_data = 0;
bool ready = false;

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

// Timed mutex attempt (fixed using timed_mutex)
void timed_try_increment() {

    if (timed_mtx.try_lock_for(
            std::chrono::milliseconds(10))) {

        shared_data += 15;

        std::cout << "timed lock succeeded\n";

        timed_mtx.unlock();

    } else {

        std::cout << "timed lock failed\n";
    }
}

// RAII-style manual lock/unlock demo
void manual_lock_demo() {

    mtx.lock();

    shared_data += 5;

    std::cout << "manual lock used\n";

    mtx.unlock();
}

// Thread-safe print helper
std::mutex cout_mtx;

void safe_print(const std::string& msg) {

    std::lock_guard<std::mutex> lock(cout_mtx);

    std::cout << msg << "\n";
}

// 🔹 NEW: unique_lock with try_lock_for
void unique_lock_timed() {

    std::unique_lock<std::timed_mutex>
        lock(timed_mtx, std::defer_lock);

    if (lock.try_lock_for(
            std::chrono::milliseconds(5))) {

        shared_data += 25;

        std::cout << "unique_lock timed success\n";

    } else {

        std::cout << "unique_lock timed failed\n";
    }
}

// 🔹 NEW: scoped thread-safe increment loop
void batch_increment(int times) {

    for (int i = 0; i < times; ++i) {

        std::lock_guard<std::mutex> lock(mtx);

        shared_data++;
    }
}

// ---------------- EXTRA ADDITIONS ----------------

// 🔹 NEW: shared_mutex reader
void reader_task() {

    std::shared_lock lock(shared_mtx);

    std::cout << "Reader sees value: "
              << shared_data << "\n";
}

// 🔹 NEW: shared_mutex writer
void writer_task() {

    std::unique_lock lock(shared_mtx);

    shared_data += 100;

    std::cout << "Writer updated value\n";
}

// 🔹 NEW: condition variable waiter
void wait_task() {

    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, []() {
        return ready;
    });

    std::cout << "Condition variable triggered\n";
}

// 🔹 NEW: condition variable notifier
void notify_task() {

    {
        std::lock_guard<std::mutex> lock(mtx);

        ready = true;
    }

    cv.notify_one();
}

// 🔹 NEW: recursive mutex demo
std::recursive_mutex rec_mtx;

void recursive_function(int depth) {

    std::lock_guard<std::recursive_mutex>
        lock(rec_mtx);

    std::cout << "Recursive depth: "
              << depth << "\n";

    if (depth > 0) {
        recursive_function(depth - 1);
    }
}

// 🔹 NEW: deadlock-safe locking
void deadlock_safe_demo() {

    std::lock(mtx, mtx2);

    std::lock_guard<std::mutex>
        lock1(mtx, std::adopt_lock);

    std::lock_guard<std::mutex>
        lock2(mtx2, std::adopt_lock);

    shared_data += 30;

    std::cout << "Deadlock-safe locking used\n";
}

// ---------------- MAIN ----------------

int main() {

    std::vector<std::thread> threads;

    // lock_guard threads
    for (int i = 0; i < 5; ++i) {

        threads.emplace_back(increment_safe);
    }

    // unique_lock thread
    threads.emplace_back(
        increment_with_unique_lock);

    // try_lock thread
    threads.emplace_back(try_increment);

    // scoped_lock thread
    threads.emplace_back(safe_dual_lock);

    // ---------------- ADDED THREADS ----------------

    threads.emplace_back(timed_try_increment);

    threads.emplace_back(manual_lock_demo);

    // Safe print demo
    threads.emplace_back([]() {

        safe_print(
            "Thread-safe printing example");
    });

    // 🔹 NEW threads
    threads.emplace_back(unique_lock_timed);

    threads.emplace_back(batch_increment, 50);

    // ---------------- EXTRA THREADS ----------------

    threads.emplace_back(reader_task);

    threads.emplace_back(writer_task);

    threads.emplace_back(wait_task);

    threads.emplace_back(notify_task);

    threads.emplace_back(deadlock_safe_demo);

    for (auto& t : threads) {
        t.join();
    }

    // Recursive mutex demo
    std::cout << "\n--- Recursive Mutex Demo ---\n";

    recursive_function(3);

    std::cout << "\nFinal value: "
              << shared_data << "\n";

    return 0;
}
