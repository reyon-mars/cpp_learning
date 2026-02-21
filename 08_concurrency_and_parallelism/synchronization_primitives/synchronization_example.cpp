// Synchronization Primitives Exercise
// mutex, lock_guard, unique_lock

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>

std::mutex mtx;
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

int main() {
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(increment_safe);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "Final value: " << shared_data << "\n";
    
    return 0;
}
