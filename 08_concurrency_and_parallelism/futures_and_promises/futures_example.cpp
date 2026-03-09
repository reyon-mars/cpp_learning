// Futures and Promises Exercise
// Asynchronous value passing

#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <stdexcept>

int calculate_sum(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return a + b;
}

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

    return 0;
}
