// Async Programming Exercise
// std::async and packaged_task

#include <iostream>
#include <future>
#include <thread>
#include <chrono>

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

    return 0;
}
