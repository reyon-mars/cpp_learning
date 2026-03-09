// Atomics and Memory Ordering Exercise
// Atomic operations and memory barriers

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <chrono>

std::atomic<int> counter(0);
std::atomic<bool> ready(false);

// Spinlock using atomic_flag
std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;

void spinlock() {
    while (lock_flag.test_and_set(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
}

void unlock() {
    lock_flag.clear(std::memory_order_release);
}

void worker() {
    // Wait until main thread signals readiness
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    for (int i = 0; i < 100; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

void protected_increment() {
    spinlock();
    counter++;
    unlock();
}

int main() {

    std::thread t1(worker);
    std::thread t2(worker);

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    ready.store(true, std::memory_order_release);

    t1.join();
    t2.join();

    std::cout << "Final counter (relaxed): "
              << counter.load(std::memory_order_seq_cst) << "\n";


    // ---------------- compare_exchange example ----------------

    int expected = counter.load();
    int desired = expected + 10;

    if (counter.compare_exchange_strong(expected, desired)) {
        std::cout << "compare_exchange succeeded\n";
    } else {
        std::cout << "compare_exchange failed\n";
    }

    std::cout << "Counter after CAS: " << counter.load() << "\n";


    // ---------------- spinlock example ----------------

    std::vector<std::thread> threads;

    for (int i = 0; i < 5; ++i) {
        threads.emplace_back(protected_increment);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Counter after spinlock increments: "
              << counter.load() << "\n";

    return 0;
}
