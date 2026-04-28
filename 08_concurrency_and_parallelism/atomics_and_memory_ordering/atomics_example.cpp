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

// ---------------- SMALL ADDITIONS ----------------

// Demonstrate sequential consistency
void seq_cst_increment() {
    counter.fetch_add(1, std::memory_order_seq_cst);
}

// Demonstrate acquire-release pair
std::atomic<int> shared_data(0);

void producer() {
    shared_data.store(42, std::memory_order_release);
    ready.store(true, std::memory_order_release);
}

void consumer() {
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }
    std::cout << "Consumer sees value: "
              << shared_data.load(std::memory_order_acquire) << "\n";
}

// ---------------- EXTRA ADDITIONS ----------------

// Relaxed load/store demo
void relaxed_demo() {
    std::atomic<int> x(0);

    std::thread t1([&]() {
        x.store(10, std::memory_order_relaxed);
    });

    std::thread t2([&]() {
        std::cout << "Relaxed read (may be 0 or 10): "
                  << x.load(std::memory_order_relaxed) << "\n";
    });

    t1.join();
    t2.join();
}

// compare_exchange_weak loop example
void cas_loop_demo() {
    int expected = counter.load();
    while (!counter.compare_exchange_weak(expected, expected + 1)) {
        // retry until success
    }
    std::cout << "CAS loop increment done\n";
}

// Memory fence example
void fence_demo() {
    std::atomic<int> a(0), b(0);

    std::thread t1([&]() {
        a.store(1, std::memory_order_relaxed);
        std::atomic_thread_fence(std::memory_order_release);
        b.store(1, std::memory_order_relaxed);
    });

    std::thread t2([&]() {
        while (b.load(std::memory_order_relaxed) == 0);
        std::atomic_thread_fence(std::memory_order_acquire);
        std::cout << "Fence ensures a = "
                  << a.load(std::memory_order_relaxed) << "\n";
    });

    t1.join();
    t2.join();
}

// ------------------------------------------------

// ---------------- MAIN ----------------

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


    // ---------------- ADDED USAGE ----------------

    // Sequential consistency demo
    seq_cst_increment();
    std::cout << "Counter after seq_cst increment: "
              << counter.load() << "\n";

    // Producer-consumer demo
    ready.store(false);  // reset flag

    std::thread prod(producer);
    std::thread cons(consumer);

    prod.join();
    cons.join();

    // ---------------- EXTRA USAGE ----------------

    std::cout << "\n--- Extra Tests ---\n";

    // Relaxed ordering demo
    relaxed_demo();

    // CAS loop demo
    cas_loop_demo();
    std::cout << "Counter after CAS loop: "
              << counter.load() << "\n";

    // Memory fence demo
    fence_demo();

    // ------------------------------------------------

    return 0;
}
