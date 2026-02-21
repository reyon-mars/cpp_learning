// Atomics and Memory Ordering Exercise
// Atomic operations and memory barriers

#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> counter(0);
std::atomic<bool> ready(false);

void worker() {
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
    
    for (int i = 0; i < 100; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    std::thread t1(worker);
    std::thread t2(worker);
    
    ready.store(true, std::memory_order_release);
    
    t1.join();
    t2.join();
    
    std::cout << "Final counter: " << counter.load() << "\n";
    
    return 0;
}
