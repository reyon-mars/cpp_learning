// Async Programming Exercise
// std::async and packaged_task

#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>      // ✅ ADDED
#include <numeric>     // 🔹 ADDED
#include <functional>  // 🔹 ADDED
#include <mutex>       // 🔹 ADDED

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

// ======================================================
// 🔥 NEW SMALL ADDITIONS
// ======================================================

// Parallel sum using async
int parallel_sum(const std::vector<int>& values) {

    auto mid = values.size() / 2;

    auto future_left = std::async(std::launch::async, [&]() {
        return std::accumulate(values.begin(),
                               values.begin() + mid,
                               0);
    });

    auto future_right = std::async(std::launch::async, [&]() {
        return std::accumulate(values.begin() + mid,
                               values.end(),
                               0);
    });

    return future_left.get() + future_right.get();
}

// Async factorial
std::future<int> async_factorial(int n) {
    return std::async(std::launch::async, [n]() {
        int result = 1;
        for (int i = 1; i <= n; ++i)
            result *= i;
        return result;
    });
}

// Mutex-protected async printing
std::mutex cout_mutex;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << msg << "\n";
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

    // ======================================================
    // 🔥 NEW ADVANCED USAGE
    // ======================================================

    std::cout << "\n--- Advanced Async Features ---\n";

    // Parallel sum demo
    std::vector<int> nums = {1,2,3,4,5,6,7,8,9,10};

    std::cout << "Parallel sum result: "
              << parallel_sum(nums) << "\n";

    // Async factorial
    auto factorial_future = async_factorial(5);

    std::cout << "Factorial async result: "
              << factorial_future.get() << "\n";

    // Async safe printing
    auto printer1 = std::async(std::launch::async, []() {
        safe_print("Async printer 1 executed");
    });

    auto printer2 = std::async(std::launch::async, []() {
        safe_print("Async printer 2 executed");
    });

    printer1.wait();
    printer2.wait();

    // Chained async computation
    auto chained = std::async(std::launch::async, []() {
        int value = heavy_computation(50);
        return value * 2;
    });

    std::cout << "Chained async result: "
              << chained.get() << "\n";

    // ======================================================

    return 0;
}
