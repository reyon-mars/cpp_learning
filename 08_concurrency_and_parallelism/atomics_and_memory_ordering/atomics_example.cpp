#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <vector>
#include <numeric>
#include <functional>
#include <mutex>
#include <concepts>
#include <format>
#include <span>
#include <ranges>
#include <stop_token>
#include <latch>
#include <barrier>
#include <semaphore>
#include <atomic>
#include <optional>

[[nodiscard]] int heavy_computation(int n) noexcept {
    return n * (n + 1) / 2;
}

[[nodiscard]] int delayed_task(int n) {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return heavy_computation(n);
}

[[nodiscard]] std::future<int> safe_async_call(int n) {
    return std::async(std::launch::async, [n]() noexcept {
        return heavy_computation(n);
    });
}

[[nodiscard]] std::future<int> promise_example(int n) {
    std::promise<int> prom;
    auto fut = prom.get_future();

    std::thread([n, p = std::move(prom)]() mutable {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        p.set_value(heavy_computation(n));
    }).detach();

    return fut;
}

void shared_future_demo() {
    std::promise<int> prom;
    std::shared_future<int> sf = prom.get_future().share();

    std::thread([p = std::move(prom)]() mutable {
        p.set_value(123);
    }).detach();

    const auto val = sf.get();
    std::cout << std::format("Shared future value 1: {}\n", val);
    std::cout << std::format("Shared future value 2: {}\n", val);
}

void timed_wait_demo() {
    auto fut = std::async(std::launch::async, []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        return 77;
    });

    if (fut.wait_for(std::chrono::milliseconds(50)) == std::future_status::timeout)
        std::cout << "Still waiting...\n";

    std::cout << std::format("Timed wait result: {}\n", fut.get());
}

[[nodiscard]] int parallel_sum(std::span<const int> values) {
    if (values.empty()) return 0;

    const auto mid = values.size() / 2;

    auto left  = std::async(std::launch::async, [=] {
        return std::reduce(values.begin(), values.begin() + mid, 0);
    });
    auto right = std::async(std::launch::async, [=] {
        return std::reduce(values.begin() + mid, values.end(), 0);
    });

    return left.get() + right.get();
}

[[nodiscard]] std::future<long long> async_factorial(int n) {
    return std::async(std::launch::async, [n]() noexcept -> long long {
        long long result = 1;
        for (int i = 2; i <= n; ++i)
            result *= i;
        return result;
    });
}

std::mutex cout_mutex;

void safe_print(std::string_view msg) {
    std::lock_guard lock(cout_mutex);
    std::cout << std::format("{}\n", msg);
}

template <std::invocable<> Fn>
[[nodiscard]] std::future<std::invoke_result_t<Fn>> async_guarded(Fn&& fn) {
    return std::async(std::launch::async,
        [f = std::forward<Fn>(fn)]() mutable -> std::invoke_result_t<Fn> {
            try {
                return std::invoke(f);
            } catch (const std::exception& e) {
                safe_print(std::format("Exception in async_guarded: {}", e.what()));
                if constexpr (std::is_same_v<std::invoke_result_t<Fn>, int>)
                    return -1;
                else
                    throw;
            }
        }
    );
}

template <std::invocable<> Fn>
[[nodiscard]] auto batch_async(std::span<Fn> fns) {
    using R = std::invoke_result_t<Fn>;
    std::vector<std::future<R>> futures;
    futures.reserve(fns.size());
    for (auto& fn : fns)
        futures.emplace_back(std::async(std::launch::async, std::move(fn)));
    return futures;
}

void latch_demo() {
    constexpr int worker_count = 3;
    std::latch ready(worker_count);
    std::atomic<int> total{0};

    std::vector<std::jthread> workers;
    workers.reserve(worker_count);

    for (int i = 1; i <= worker_count; ++i) {
        workers.emplace_back([&, i] {
            total.fetch_add(heavy_computation(i * 10), std::memory_order_relaxed);
            ready.count_down();
        });
    }

    ready.wait();
    std::cout << std::format("Latch demo total: {}\n", total.load());
}

void semaphore_demo() {
    std::counting_semaphore<3> sem(3);
    std::atomic<int> concurrent{0};
    std::atomic<int> max_concurrent{0};

    auto task = [&](int id) {
        sem.acquire();
        const int c = concurrent.fetch_add(1, std::memory_order_acq_rel) + 1;
        int expected = max_concurrent.load(std::memory_order_relaxed);
        while (c > expected &&
               !max_concurrent.compare_exchange_weak(expected, c,
                                                      std::memory_order_relaxed));
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        concurrent.fetch_sub(1, std::memory_order_acq_rel);
        sem.release();
        safe_print(std::format("Semaphore task {} done", id));
    };

    std::vector<std::jthread> threads;
    threads.reserve(5);
    for (int i = 1; i <= 5; ++i)
        threads.emplace_back(task, i);
}

int main() {
    auto future1 = std::async(std::launch::async, heavy_computation, 1000);
    auto future2 = std::async(std::launch::async, heavy_computation, 2000);

    std::cout << "Computations running asynchronously...\n";

    if (future1.wait_for(std::chrono::milliseconds(10)) == std::future_status::timeout)
        std::cout << "future1 still running...\n";

    std::cout << std::format("Result 1: {}\n", future1.get());
    std::cout << std::format("Result 2: {}\n", future2.get());

    std::cout << "\n--- packaged_task ---\n";
    {
        std::packaged_task<int(int)> task(heavy_computation);
        auto future3 = task.get_future();
        std::thread worker(std::move(task), 500);
        std::cout << "Waiting for packaged_task result...\n";
        std::cout << std::format("Packaged task result: {}\n", future3.get());
        worker.join();
    }

    std::cout << "\n--- lambda async ---\n";
    {
        auto future4 = std::async(std::launch::async, []() noexcept {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            return 42;
        });
        std::cout << std::format("Lambda async result: {}\n", future4.get());
    }

    std::cout << "\n--- delayed & safe async ---\n";
    std::cout << std::format("Delayed task result: {}\n",
                             std::async(std::launch::async, delayed_task, 300).get());
    std::cout << std::format("Safe async result: {}\n", safe_async_call(400).get());

    std::cout << "\n--- batch async ---\n";
    {
        std::vector<std::future<int>> futures;
        futures.reserve(3);
        for (int i : {100, 200, 300})
            futures.emplace_back(std::async(std::launch::async, heavy_computation, i));

        std::cout << "Batch async results: ";
        for (auto& f : futures)
            std::cout << std::format("{} ", f.get());
        std::cout << '\n';
    }

    std::cout << "\n--- Extra Tests ---\n";

    std::cout << std::format("Promise result: {}\n", promise_example(150).get());
    shared_future_demo();
    timed_wait_demo();

    std::cout << "\n--- Advanced Async Features ---\n";

    const std::vector<int> nums = {1,2,3,4,5,6,7,8,9,10};
    std::cout << std::format("Parallel sum result: {}\n", parallel_sum(nums));

    std::cout << std::format("Factorial async result: {}\n", async_factorial(5).get());

    {
        auto p1 = std::async(std::launch::async, [] { safe_print("Async printer 1 executed"); });
        auto p2 = std::async(std::launch::async, [] { safe_print("Async printer 2 executed"); });
        p1.wait();
        p2.wait();
    }

    {
        auto chained = async_guarded([] { return heavy_computation(50) * 2; });
        std::cout << std::format("Chained async result: {}\n", chained.get());
    }

    std::cout << "\n--- C++20 Latch ---\n";
    latch_demo();

    std::cout << "\n--- C++20 Semaphore ---\n";
    semaphore_demo();

    std::cout << "\n--- jthread with stop_token ---\n";
    {
        std::atomic<int> count{0};
        std::jthread worker([&](std::stop_token st) {
            while (!st.stop_requested()) {
                count.fetch_add(1, std::memory_order_relaxed);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(55));
        worker.request_stop();
        worker.join();
        std::cout << std::format("jthread incremented {} times before stop\n", count.load());
    }

    return 0;
}
