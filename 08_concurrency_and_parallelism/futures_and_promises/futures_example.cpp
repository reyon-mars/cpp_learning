// Futures and Promises Exercise
// Asynchronous value passing

#include <iostream>
#include <thread>
#include <future>

int calculate_sum(int a, int b) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return a + b;
}

int main() {
    // Using std::promise and std::future
    std::promise<int> promise;
    std::future<int> future = promise.get_future();
    
    std::thread t([&promise]() {
        int result = calculate_sum(10, 20);
        promise.set_value(result);
    });
    
    std::cout << "Waiting for result...\n";
    int result = future.get();
    std::cout << "Result: " << result << "\n";
    
    t.join();
    
    return 0;
}
