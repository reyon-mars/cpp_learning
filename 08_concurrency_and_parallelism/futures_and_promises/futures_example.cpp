#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <stdexcept>
#include <vector>
#include <numeric>
#include <utility>

using namespace std::chrono_literals;

int calculate_sum(int a, int b) {
    std::this_thread::sleep_for(100ms);
    return a + b;
}

int risky_division(int a, int b) {
    if (b == 0) throw std::runtime_error{"Division by zero"};
    return a / b;
}

void set_after(std::promise<int>& p, int value, std::chrono::milliseconds delay = 0ms) {
    std::this_thread::sleep_for(delay);
    p.set_value(value);
}

template<typename Future>
void check_status(Future& fut, std::string_view label = "Future") {
    auto status = fut.wait_for(10ms);
    if (status == std::future_status::timeout)
        std::cout << label << " still running\n";
    else
        std::cout << label << " ready\n";
}

[[nodiscard]] std::future<int> async_square(int n) {
    return std::async(std::launch::async, [n] { return n * n; });
}

[[nodiscard]] std::future<int> delayed_future(int value, std::chrono::milliseconds delay = 80ms) {
    return std::async(std::launch::async, [value, delay] {
        std::this_thread::sleep_for(delay);
        return value;
    });
}

void shared_future_reader(std::shared_future<int> sf, int id) {
    std::cout << "Reader " << id << " got=" << sf.get() << "\n";
}

template<typename Func>
[[nodiscard]] std::pair<std::future<int>, std::thread>
make_promised(Func fn) {
    std::promise<int> p;
    auto fut = p.get_future();
    std::thread t{[p = std::move(p), fn = std::move(fn)]() mutable {
        try { p.set_value(fn()); }
        catch (...) { p.set_exception(std::current_exception()); }
    }};
    return {std::move(fut), std::move(t)};
}

int main() {
    std::cout << "=== promise + future (manual thread) ===\n";
    {
        auto [fut, t] = make_promised([] { return calculate_sum(10, 20); });
        std::cout << "Waiting...\n";
        std::cout << "Result=" << fut.get() << "\n";
        t.join();
    }

    std::cout << "\n=== shared_future (one producer, two consumers) ===\n";
    {
        std::promise<int> p;
        std::shared_future<int> sf = p.get_future().share();

        std::thread producer{[&p]{ std::this_thread::sleep_for(100ms); p.set_value(50); }};
        std::thread c1{[sf]{ std::cout << "Consumer1 got=" << sf.get() << "\n"; }};
        std::thread c2{[sf]{ std::cout << "Consumer2 got=" << sf.get() << "\n"; }};

        producer.join(); c1.join(); c2.join();
    }

    std::cout << "\n=== exception propagation ===\n";
    {
        auto [fut, t] = make_promised([] { return risky_division(10, 0); });
        try {
            std::cout << "Result=" << fut.get() << "\n";
        } catch (const std::exception& e) {
            std::cout << "Caught: " << e.what() << "\n";
        }
        t.join();
    }

    std::cout << "\n=== check_status before ready ===\n";
    {
        auto [fut, t] = make_promised([] {
            std::this_thread::sleep_for(50ms);
            return 6 * 7;
        });
        check_status(fut, "multiply");
        std::cout << "Result=" << fut.get() << "\n";
        t.join();
    }

    std::cout << "\n=== chained computation ===\n";
    {
        auto [fut, t] = make_promised([] { return calculate_sum(2, 3) * 10; });
        std::cout << "Chained=" << fut.get() << "\n";
        t.join();
    }

    std::cout << "\n=== async_square batch ===\n";
    {
        std::vector<std::future<int>> futures;
        futures.reserve(5);
        for (int i = 1; i <= 5; ++i) futures.push_back(async_square(i));
        std::cout << "Squares: ";
        for (auto& f : futures) std::cout << f.get() << " ";
        std::cout << "\n";
    }

    std::cout << "\n=== delayed_future ===\n";
    {
        auto fut = delayed_future(999, 80ms);
        check_status(fut, "delayed");
        std::cout << "Delayed=" << fut.get() << "\n";
    }

    std::cout << "\n=== shared_future (three readers) ===\n";
    {
        std::promise<int> p;
        std::shared_future<int> sf = p.get_future().share();

        std::thread writer{[&p]{ std::this_thread::sleep_for(50ms); p.set_value(777); }};
        std::thread r1{shared_future_reader, sf, 1};
        std::thread r2{shared_future_reader, sf, 2};
        std::thread r3{shared_future_reader, sf, 3};

        writer.join(); r1.join(); r2.join(); r3.join();
    }

    std::cout << "\n=== combine two std::async results ===\n";
    {
        auto f1 = std::async(std::launch::async, calculate_sum, 1, 2);
        auto f2 = std::async(std::launch::async, calculate_sum, 3, 4);
        std::cout << "Combined=" << f1.get() + f2.get() << "\n";
    }

    return 0;
}
