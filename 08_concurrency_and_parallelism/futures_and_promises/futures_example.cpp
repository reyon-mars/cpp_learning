// Futures and Promises Exercise
// Asynchronous value passing

#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <stdexcept>
#include <vector>     // 🔹 ADDED
#include <numeric>    // 🔹 ADDED

// ---------------- ORIGINAL FUNCTION ----------------

int calculate_sum(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return a + b;
}

// ---------------- SMALL ADDITIONS ----------------

// Function that may throw
int risky_division(int a, int b) {
    if (b == 0)
        throw std::runtime_error("Division by zero");
    return a / b;
}

// Helper to run async task with promise
void async_task(std::promise<int> p, int a, int b) {
    try {
        p.set_value(calculate_sum(a, b));
    } catch (...) {
        p.set_exception(std::current_exception());
    }
}

// 🔹 NEW: multiply task
void async_multiply(std::promise<int> p, int a, int b) {
    try {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        p.set_value(a * b);
    } catch (...) {
        p.set_exception(std::current_exception());
    }
}

// 🔹 NEW: future status checker
void check_status(std::future<int>& fut) {
    if (fut.wait_for(std::chrono::milliseconds(10)) == std::future_status::timeout) {
        std::cout << "Future still running...\n";
    } else {
        std::cout << "Future ready!\n";
    }
}

// ---------------- EXTRA ADDITIONS ----------------

// 🔹 NEW: async subtraction task
void async_subtract(std::promise<int> p, int a, int b) {
    try {
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
        p.set_value(a - b);
    } catch (...) {
        p.set_exception(std::current_exception());
    }
}

// 🔹 NEW: batch async calculator
std::future<int> async_square(int n) {
    return std::async(std::launch::async, [n]() {
        return n * n;
    });
}

// 🔹 NEW: delayed future demo
std::future<int> delayed_future(int value) {
    return std::async(std::launch::async, [value]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(80));
        return value;
    });
}

// 🔹 NEW: shared_future helper
void shared_future_reader(std::shared_future<int> sf, int id) {
    std::cout << "Reader " << id
              << " received: " << sf.get() << "\n";
}

// ---------------- MAIN ----------------

int main() {

    // Using std::promise and std::future
    std::promise<int> promise;
    std::future<int> future = promise.get_future();

    std::thread t([&promise]() {
        try {
            int result = calculate_sum(10, 20);
            promise.set_value(result);
        } catch (...) {
            promise.set_exception(std::current_exception());
        }
    });

    std::cout << "Waiting for result...\n";

    future.wait(); // wait explicitly
    int result = future.get();

    std::cout << "Result: " << result << "\n";

    t.join();


    // -------- shared_future example --------

    std::promise<int> promise2;
    std::future<int> future2 = promise2.get_future();
    std::shared_future<int> shared = future2.share();

    std::thread producer([&promise2]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        promise2.set_value(50);
    });

    std::thread consumer1([shared]() {
        std::cout << "Consumer 1 got: " << shared.get() << "\n";
    });

    std::thread consumer2([shared]() {
        std::cout << "Consumer 2 got: " << shared.get() << "\n";
    });

    producer.join();
    consumer1.join();
    consumer2.join();


    // ---------------- ADDED USAGE ----------------

    // Using helper async_task
    std::promise<int> promise3;
    std::future<int> future3 = promise3.get_future();
    std::thread t2(async_task, std::move(promise3), 5, 15);

    std::cout << "Async task result: " << future3.get() << "\n";
    t2.join();

    // Exception propagation demo
    std::promise<int> promise4;
    std::future<int> future4 = promise4.get_future();

    std::thread t3([&promise4]() {
        try {
            promise4.set_value(risky_division(10, 0));
        } catch (...) {
            promise4.set_exception(std::current_exception());
        }
    });

    try {
        std::cout << "Risky result: " << future4.get() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Caught exception from thread: "
                  << e.what() << "\n";
    }

    t3.join();

    // 🔹 NEW: async multiply demo
    std::promise<int> promise5;
    std::future<int> future5 = promise5.get_future();
    std::thread t4(async_multiply, std::move(promise5), 6, 7);

    check_status(future5);
    std::cout << "Multiply result: " << future5.get() << "\n";
    t4.join();

    // 🔹 NEW: chaining futures (manual)
    std::promise<int> promise6;
    std::future<int> future6 = promise6.get_future();

    std::thread t5([&promise6]() {
        try {
            int val = calculate_sum(2, 3);
            val *= 10; // chained logic
            promise6.set_value(val);
        } catch (...) {
            promise6.set_exception(std::current_exception());
        }
    });

    std::cout << "Chained result: " << future6.get() << "\n";
    t5.join();

    // ======================================================
    // 🔥 EXTRA NEW ADDITIONS
    // ======================================================

    std::cout << "\n--- Extra Future Tests ---\n";

    // 🔹 async subtraction
    std::promise<int> promise7;
    std::future<int> future7 = promise7.get_future();

    std::thread t6(async_subtract, std::move(promise7), 20, 8);

    std::cout << "Subtract result: "
              << future7.get() << "\n";

    t6.join();

    // 🔹 async square batch
    std::vector<std::future<int>> futures;

    for (int i = 1; i <= 5; ++i) {
        futures.push_back(async_square(i));
    }

    std::cout << "Square results: ";
    for (auto& f : futures) {
        std::cout << f.get() << " ";
    }
    std::cout << "\n";

    // 🔹 delayed future demo
    auto delayed = delayed_future(999);

    if (delayed.wait_for(std::chrono::milliseconds(20))
        == std::future_status::timeout) {
        std::cout << "Delayed future still running...\n";
    }

    std::cout << "Delayed future result: "
              << delayed.get() << "\n";

    // 🔹 shared_future with multiple readers
    std::promise<int> promise8;
    std::shared_future<int> shared2 =
        promise8.get_future().share();

    std::thread writer([&promise8]() {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(50));
        promise8.set_value(777);
    });

    std::thread r1(shared_future_reader, shared2, 1);
    std::thread r2(shared_future_reader, shared2, 2);
    std::thread r3(shared_future_reader, shared2, 3);

    writer.join();
    r1.join();
    r2.join();
    r3.join();

    // 🔹 combine async results
    auto f1 = std::async(std::launch::async,
                         calculate_sum, 1, 2);

    auto f2 = std::async(std::launch::async,
                         calculate_sum, 3, 4);

    int combined = f1.get() + f2.get();

    std::cout << "Combined async total: "
              << combined << "\n";

    // ======================================================

    return 0;
}
