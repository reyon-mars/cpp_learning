// Synchronization Primitives Exercise
// mutex, lock_guard, unique_lock

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
std::mutex mtx2;
int shared_data = 0;

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

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Final value: " << shared_data << "\n";

    return 0;
}
