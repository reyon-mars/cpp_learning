// Async Programming Exercise
// std::async and packaged_task

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>   // ✅ ADDED

int heavy_computation(int n) {
    int sum = 0;
    for (int i = 0; i <= n; ++i) {
        sum += i;
    }
    return sum;
}

// ---------------- SMALL ADDITIONS ----------------

// Delayed computation
int delayed_task(int n) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return heavy_computation(n);
}

// Safe async wrapper
std::future<int> safe_async_call(int n) {
    return std::async(std::launch::async, [n]() {
        try {
            return heavy_computation(n);
        } catch (...) {
            std::cout << "Exception in async task\n";
            return -1;
        }
    });
}

// ---------------- EXTRA ADDITIONS ----------------

// Promise example
std::future<int> promise_example(int n) {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread([n, p = std::move(prom)]() mutable {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        p.set_value(heavy_computation(n));
    }).detach();

    return fut;
}

// Shared future example
void shared_future_demo() {
    std::promise<int> prom;
    std::shared_future<int> sf = prom.get_future().share();

    std::thread([p = std::move(prom)]() mutable {
        p.set_value(123);
    }).detach();

    std::cout << "Shared future value 1: " << sf.get() << "\n";
    std::cout << "Shared future value 2: " << sf.get() << "\n";
}

// Timed wait example
void timed_wait_demo() {
    auto fut = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        return 77;
    });

    if (fut.wait_for(std::chrono::milliseconds(50)) == std::future_status::timeout) {
        std::cout << "Still waiting...\n";
    }

    std::cout << "Timed wait result: " << fut.get() << "\n";
}

// ------------------------------------------------

// ---------------- MAIN ----------------

int main() {

    // Using std::async
    auto future1 = std::async(std::launch::async, heavy_computation, 1000);
    auto future2 = std::async(std::launch::async, heavy_computation, 2000);

    std::cout << "Computations running asynchronously...\n";

    // Check status of future
    if (future1.wait_for(std::chrono::milliseconds(10)) == std::future_status::timeout) {
        std::cout << "future1 still running...\n";
    }

    int result1 = future1.get();
    int result2 = future2.get();

    std::cout << "Result 1: " << result1 << "\n";
    std::cout << "Result 2: " << result2 << "\n";


    // ---------------- packaged_task example ----------------

    std::packaged_task<int(int)> task(heavy_computation);
    std::future<int> future3 = task.get_future();

    std::thread worker(std::move(task), 500);

    std::cout << "Waiting for packaged_task result...\n";
    future3.wait();

    std::cout << "Packaged task result: " << future3.get() << "\n";

    worker.join();


    // ---------------- Lambda async example ----------------

    auto future4 = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return 42;
    });

    std::cout << "Lambda async result: " << future4.get() << "\n";


    // ---------------- ADDED USAGE ----------------

    // Delayed async task
    auto future5 = std::async(std::launch::async, delayed_task, 300);
    std::cout << "Delayed task result: " << future5.get() << "\n";

    // Safe async wrapper demo
    auto future6 = safe_async_call(400);
    std::cout << "Safe async result: " << future6.get() << "\n";

    // Multiple async tasks in loop
    std::vector<std::future<int>> futures;
    for (int i = 1; i <= 3; ++i) {
        futures.push_back(std::async(std::launch::async, heavy_computation, i * 100));
    }

    std::cout << "Batch async results: ";
    for (auto& f : futures) {
        std::cout << f.get() << " ";
    }
    std::cout << "\n";

    // ---------------- EXTRA USAGE ----------------

    std::cout << "\n--- Extra Tests ---\n";

    // Promise demo
    auto fut_prom = promise_example(150);
    std::cout << "Promise result: " << fut_prom.get() << "\n";

    // Shared future demo
    shared_future_demo();

    // Timed wait demo
    timed_wait_demo();

    // ------------------------------------------------

    return 0;
}
