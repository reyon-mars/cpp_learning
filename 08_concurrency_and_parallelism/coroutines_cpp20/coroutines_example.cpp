#include <coroutine>
#include <exception>
#include <iostream>
#include <vector>
#include <string_view>
#include <string>
#include <optional>
#include <utility>
#include <ranges>
#include <numeric>
#include <format>

class SimpleCoroutine {
public:
    struct promise_type {
        int value = 0;

        SimpleCoroutine get_return_object() {
            return SimpleCoroutine{Handle::from_promise(*this)};
        }

        std::suspend_never  initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend()   noexcept { return {}; }

        void return_value(int v) noexcept { value = v; }
        void unhandled_exception() noexcept { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit SimpleCoroutine(Handle h) noexcept : handle_{h} {}

    SimpleCoroutine(const SimpleCoroutine&)            = delete;
    SimpleCoroutine& operator=(const SimpleCoroutine&) = delete;

    SimpleCoroutine(SimpleCoroutine&& other) noexcept
        : handle_{std::exchange(other.handle_, {})} {}

    SimpleCoroutine& operator=(SimpleCoroutine&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    ~SimpleCoroutine() { if (handle_) handle_.destroy(); }

    [[nodiscard]] int  get()  const noexcept { return handle_.promise().value; }
    [[nodiscard]] bool done() const noexcept { return !handle_ || handle_.done(); }

    void resume() {
        if (!done()) handle_.resume();
        else         std::cout << "[already done]\n";
    }

private:
    Handle handle_;
};

template<typename T>
class Generator {
public:
    struct promise_type {
        std::optional<T> current;

        Generator get_return_object() {
            return Generator{Handle::from_promise(*this)};
        }

        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend()   noexcept { return {}; }

        std::suspend_always yield_value(T v) noexcept {
            current = std::move(v);
            return {};
        }

        void return_void() noexcept {}
        void unhandled_exception() noexcept { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Generator(Handle h) noexcept : handle_{h} {}

    Generator(const Generator&)            = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator&& other) noexcept : handle_{std::exchange(other.handle_, {})} {}

    ~Generator() { if (handle_) handle_.destroy(); }

    [[nodiscard]] bool next() {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }

    [[nodiscard]] const T& value() const noexcept { return *handle_.promise().current; }

    struct Sentinel {};

    struct Iterator {
        Generator* gen;
        bool       done;

        Iterator& operator++() {
            done = !gen->next();
            return *this;
        }

        [[nodiscard]] const T& operator*() const { return gen->value(); }
        [[nodiscard]] bool operator==(Sentinel) const noexcept { return done; }
    };

    [[nodiscard]] Iterator begin() {
        bool has = next();
        return Iterator{this, !has};
    }

    [[nodiscard]] Sentinel end() noexcept { return {}; }

private:
    Handle handle_;
};

void print_status(std::string_view name, const SimpleCoroutine& c) {
    std::cout << std::format("{} done={}\n", name, c.done());
}

SimpleCoroutine compute_value() {
    std::cout << "Coroutine started\n";
    co_return 42;
}

SimpleCoroutine suspended_example() {
    std::cout << "Before suspension\n";
    co_await std::suspend_always{};
    std::cout << "After resume\n";
    co_return 100;
}

SimpleCoroutine multi_suspend() {
    std::cout << "Step 1\n"; co_await std::suspend_always{};
    std::cout << "Step 2\n"; co_await std::suspend_always{};
    std::cout << "Step 3\n";
    co_return 300;
}

SimpleCoroutine quick_task() {
    co_return 7 * 6;
}

SimpleCoroutine auto_resume_task() {
    std::cout << "Auto task start\n";
    co_return 555;
}

SimpleCoroutine counting_coroutine(int limit) {
    for (int i = 1; i <= limit; ++i) {
        std::cout << std::format("Counter={}\n", i);
        co_await std::suspend_always{};
    }
    co_return limit;
}

SimpleCoroutine vector_sum_coroutine(const std::vector<int>& values) {
    int total = 0;
    for (int v : values) {
        total += v;
        std::cout << std::format("Adding {} running_total={}\n", v, total);
        co_await std::suspend_always{};
    }
    co_return total;
}

SimpleCoroutine square_coroutine(int value) {
    std::cout << std::format("Squaring {}\n", value);
    co_return value * value;
}

SimpleCoroutine chained_coroutine() {
    std::cout << "Chain start\n";
    co_await std::suspend_always{};
    std::cout << "Chain continue\n";
    co_return 999;
}

SimpleCoroutine fibonacci_coroutine(int steps) {
    int a = 0, b = 1;
    for (int i = 0; i < steps; ++i) {
        std::cout << std::format("Fib[{}]={}\n", i, a);
        co_await std::suspend_always{};
        int next = a + b;
        a = b;
        b = next;
    }
    co_return a;
}

SimpleCoroutine accumulate_coroutine(std::vector<int> values, int multiplier) {
    int total = 0;
    for (int v : values) {
        total += v * multiplier;
        co_await std::suspend_always{};
    }
    co_return total;
}

Generator<int> iota_generator(int start, int stop) {
    for (int i = start; i < stop; ++i)
        co_yield i;
}

Generator<int> fibonacci_generator(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        int next = a + b;
        a = b;
        b = next;
    }
}

Generator<int> filter_generator(Generator<int> source, int divisor) {
    while (source.next())
        if (source.value() % divisor == 0)
            co_yield source.value();
}

int main() {
    std::cout << "=== compute_value ===\n";
    auto c1 = compute_value();
    std::cout << std::format("Result={}\n", c1.get());

    std::cout << "\n=== suspended_example ===\n";
    auto c2 = suspended_example();
    print_status("c2", c2);
    c2.resume();
    std::cout << std::format("Result={}\n", c2.get());
    print_status("c2", c2);

    std::cout << "\n=== multi_suspend ===\n";
    auto c3 = multi_suspend();
    c3.resume();
    c3.resume();
    c3.resume();
    std::cout << std::format("Result={}\n", c3.get());
    c3.resume();

    std::cout << "\n=== quick_task ===\n";
    auto c4 = quick_task();
    std::cout << std::format("Result={}\n", c4.get());

    std::cout << "\n=== auto_resume_task ===\n";
    auto c5 = auto_resume_task();
    std::cout << std::format("Result={}\n", c5.get());

    std::cout << "\n=== counting_coroutine(3) ===\n";
    auto counter = counting_coroutine(3);
    print_status("counter", counter);
    counter.resume();
    counter.resume();
    counter.resume();
    std::cout << std::format("Result={}\n", counter.get());
    print_status("counter", counter);

    std::cout << "\n=== vector_sum_coroutine ===\n";
    const std::vector<int> nums{1, 2, 3, 4};
    auto vsum = vector_sum_coroutine(nums);
    for (std::size_t i = 0; i < nums.size(); ++i) vsum.resume();
    std::cout << std::format("Result={}\n", vsum.get());

    std::cout << "\n=== square_coroutine(9) ===\n";
    auto sq = square_coroutine(9);
    std::cout << std::format("Result={}\n", sq.get());

    std::cout << "\n=== chained_coroutine ===\n";
    auto chain = chained_coroutine();
    print_status("chain", chain);
    chain.resume();
    std::cout << std::format("Result={}\n", chain.get());
    print_status("chain", chain);

    std::cout << "\n=== fibonacci_coroutine(5) ===\n";
    auto fib_c = fibonacci_coroutine(5);
    for (int i = 0; i < 5; ++i) fib_c.resume();
    std::cout << std::format("Result={}\n", fib_c.get());

    std::cout << "\n=== accumulate_coroutine({1,2,3,4,5}, x3) ===\n";
    auto accum = accumulate_coroutine({1, 2, 3, 4, 5}, 3);
    for (int i = 0; i < 5; ++i) accum.resume();
    std::cout << std::format("Result={}\n", accum.get());

    std::cout << "\n=== iota_generator(0, 5) ===\n";
    for (int v : iota_generator(0, 5))
        std::cout << std::format("{} ", v);
    std::cout << '\n';

    std::cout << "\n=== fibonacci_generator(8) ===\n";
    for (int v : fibonacci_generator(8))
        std::cout << std::format("{} ", v);
    std::cout << '\n';

    std::cout << "\n=== filter_generator: multiples of 3 in [0,15) ===\n";
    for (int v : filter_generator(iota_generator(0, 15), 3))
        std::cout << std::format("{} ", v);
    std::cout << '\n';

    std::cout << "\n=== Generator sum via range-for ===\n";
    int gen_sum = 0;
    for (int v : iota_generator(1, 11)) gen_sum += v;
    std::cout << std::format("sum(1..10)={}\n", gen_sum);

    return 0;
}
