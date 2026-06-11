#include <coroutine>
#include <cstdlib>
#include <exception>
#include <optional>
#include <print>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <utility>
#include <ranges>

template<typename T>
class generator {
public:
    struct promise_type {
        std::optional<T> current_value;

        generator         get_return_object()      { return generator{handle::from_promise(*this)}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend()   noexcept { return {}; }
        void               return_void()      noexcept {}
        void               unhandled_exception() noexcept { std::terminate(); }

        std::suspend_always yield_value(T value) noexcept(std::is_nothrow_move_assignable_v<T>) {
            current_value = std::move(value);
            return {};
        }
    };

    using handle = std::coroutine_handle<promise_type>;

    explicit generator(handle h) noexcept : handle_{h} {}

    generator(const generator&)            = delete;
    generator& operator=(const generator&) = delete;

    generator(generator&& other) noexcept
        : handle_{std::exchange(other.handle_, {})} {}

    generator& operator=(generator&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    ~generator() { if (handle_) handle_.destroy(); }

    [[nodiscard]] bool resume() noexcept {
        if (!handle_ || handle_.done()) return false;
        handle_.resume();
        return !handle_.done();
    }

    [[nodiscard]] const T& value() const { return handle_.promise().current_value.value(); }

private:
    handle handle_;
};

template<typename T>
[[nodiscard]] std::vector<T> collect(generator<T>& gen) {
    std::vector<T> result;
    while (gen.resume()) result.push_back(gen.value());
    return result;
}

template<typename T>
[[nodiscard]] T sum_generator(generator<T>& gen) {
    T s{};
    while (gen.resume()) s += gen.value();
    return s;
}

template<typename T>
[[nodiscard]] T max_generator(generator<T>& gen) {
    auto values = collect(gen);
    return *std::ranges::max_element(values);
}

template<typename T>
[[nodiscard]] std::vector<T> cumulative_sum(generator<T>& gen) {
    std::vector<T> result;
    T running{};
    while (gen.resume()) {
        running += gen.value();
        result.push_back(running);
    }
    return result;
}

generator<int> counter(int from = 0, int to = 9) {
    for (int i = from; i <= to; ++i) co_yield i;
}

generator<int> squares(int n) {
    for (int i = 1; i <= n; ++i) co_yield i * i;
}

generator<int> fibonacci(int n) {
    int a = 0, b = 1;
    for (int i = 0; i < n; ++i) {
        co_yield a;
        a = std::exchange(b, a + b);
    }
}

generator<int> countdown(int start) {
    for (int i = start; i >= 0; --i) co_yield i;
}

generator<int> even_numbers(int limit) {
    for (int i = 0; i <= limit; i += 2) co_yield i;
}

generator<int> pointer_values() {
    auto ptr = std::make_unique<int>(50);
    co_yield *ptr;
    co_yield *ptr + 25;
    co_yield *ptr + 50;
}

auto main() -> int {
    std::println("=== counter (first two manual) ===");
    auto gen = counter();
    gen.resume(); std::println("value={}", gen.value());
    gen.resume(); std::println("value={}", gen.value());

    std::println("\n=== counter (remaining) ===");
    while (gen.resume()) std::println("{}", gen.value());

    std::println("\n=== squares(5) ===");
    auto sq = squares(5);
    while (sq.resume()) std::println("{}", sq.value());

    std::println("\n=== collect(counter) ===");
    auto g2 = counter();
    for (auto v : collect(g2)) std::println("{}", v);

    std::println("\n=== fibonacci(8) ===");
    auto fib = fibonacci(8);
    while (fib.resume()) std::println("{}", fib.value());

    std::println("\n=== sum_generator(counter) ===");
    auto g3 = counter();
    std::println("sum={}", sum_generator(g3));

    std::println("\n=== countdown(5) ===");
    auto down = countdown(5);
    while (down.resume()) std::println("{}", down.value());

    std::println("\n=== even_numbers(10) ===");
    auto evens = even_numbers(10);
    while (evens.resume()) std::println("{}", evens.value());

    std::println("\n=== max_generator(squares(6)) ===");
    auto g4 = squares(6);
    std::println("max={}", max_generator(g4));

    std::println("\n=== cumulative_sum(counter) ===");
    auto g5 = counter();
    for (auto v : cumulative_sum(g5)) std::println("{}", v);

    std::println("\n=== pointer_values ===");
    auto pv = pointer_values();
    while (pv.resume()) std::println("{}", pv.value());

    return EXIT_SUCCESS;
}
