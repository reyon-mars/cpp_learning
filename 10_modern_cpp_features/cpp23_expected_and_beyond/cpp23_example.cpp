#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <format>
#include <expected>
#include <functional>
#include <concepts>
#include <optional>
#include <cassert>
#include <span>
#include <type_traits>

template <typename T, typename E>
class Result {
    std::expected<T, E> impl_;

public:
    Result(const T& v) : impl_(v) {}
    Result(T&& v)      : impl_(std::move(v)) {}

    static Result ok(T v)    { return Result(std::move(v)); }
    static Result err(E e)   {
        Result r(T{});
        r.impl_ = std::unexpected(std::move(e));
        return r;
    }

    [[nodiscard]] bool success()   const noexcept { return impl_.has_value(); }
    [[nodiscard]] bool failure()   const noexcept { return !impl_.has_value(); }
    [[nodiscard]] bool has_value() const noexcept { return impl_.has_value(); }

    [[nodiscard]] const T& get_value() const & { return impl_.value(); }
    [[nodiscard]] const E& get_error() const & { return impl_.error(); }

    [[nodiscard]] T value_or(T fallback) const {
        return impl_.value_or(std::move(fallback));
    }

    template <std::invocable<T> F>
    [[nodiscard]] auto map(F&& fn) const
        -> Result<std::invoke_result_t<F, T>, E> {
        if (success())
            return Result<std::invoke_result_t<F, T>, E>(std::invoke(fn, impl_.value()));
        return Result<std::invoke_result_t<F, T>, E>::err(impl_.error());
    }

    template <std::invocable<E> F>
    [[nodiscard]] auto map_error(F&& fn) const
        -> Result<T, std::invoke_result_t<F, E>> {
        if (failure())
            return Result<T, std::invoke_result_t<F, E>>::err(std::invoke(fn, impl_.error()));
        return Result<T, std::invoke_result_t<F, E>>(impl_.value());
    }

    template <std::invocable<T> F>
    [[nodiscard]] auto and_then(F&& fn) const
        -> std::invoke_result_t<F, T> {
        if (success()) return std::invoke(fn, impl_.value());
        return std::invoke_result_t<F, T>::err(impl_.error());
    }

    void print() const {
        if (success())
            std::cout << std::format("Value: {}\n", impl_.value());
        else
            std::cout << std::format("Error: {}\n", impl_.error());
    }

    void print_state() const {
        std::cout << (success() ? "[OK]  " : "[ERR] ");
        print();
    }

    void print_compact() const {
        std::cout << std::format("{}\n", success() ? "Success" : "Failure");
    }
};

template <typename T, typename E>
Result<T, E> make_ok(T v) { return Result<T, E>::ok(std::move(v)); }

template <typename T, typename E>
Result<T, E> make_err(E e) { return Result<T, E>::err(std::move(e)); }

[[nodiscard]] Result<int, std::string> parse_int(std::string_view sv) {
    try {
        return Result<int, std::string>(std::stoi(std::string(sv)));
    } catch (...) {
        return Result<int, std::string>::err(std::format("Cannot parse '{}' as int", sv));
    }
}

[[nodiscard]] Result<int, std::string> checked_divide(int a, int b) {
    if (b == 0) return Result<int, std::string>::err("Division by zero");
    return Result<int, std::string>(a / b);
}

template <std::ranges::input_range R>
[[nodiscard]] auto range_sum(R&& r) {
    using T = std::ranges::range_value_t<R>;
    return std::reduce(std::ranges::begin(r), std::ranges::end(r), T{});
}

[[nodiscard]] double average(std::span<const int> v) noexcept {
    if (v.empty()) return 0.0;
    return static_cast<double>(range_sum(v)) / static_cast<double>(v.size());
}

void print_divider() { std::cout << "----------------------\n"; }

int main() {
    auto success = Result<int, std::string>(42);
    auto failure = Result<int, std::string>::err("Error occurred");

    if (success.success())
        std::cout << std::format("Success: {}\n", success.get_value());

    if (failure.failure())
        std::cout << std::format("Failure: {}\n", failure.get_error());

    std::cout << std::format("Value or default: {}\n", failure.value_or(0));

    success.print();
    failure.print();

    std::cout << std::format("Detected failure state: {}\n",
                             failure.failure() ? "Yes" : "No");

    auto doubled = success.map([](int x) { return x * 2; });
    std::cout << std::format("Mapped (doubled): {}\n", doubled.get_value());

    auto failed_map = failure.map([](int x) { return x * 2; });
    if (failed_map.failure())
        std::cout << std::format("Map preserved error: {}\n", failed_map.get_error());

    success.print_state();
    failure.print_state();
    print_divider();

    const std::vector<int> nums = {1, 2, 3};
    std::cout << std::format("Sum:     {}\n", range_sum(nums));
    std::cout << std::format("Average: {:.2f}\n", average(nums));
    std::cout << std::format("Max:     {}\n", *std::ranges::max_element(nums));

    auto reversed = nums | std::views::reverse;
    std::cout << "Reversed: ";
    for (int n : reversed) std::cout << std::format("{} ", n);
    std::cout << '\n';

    auto chained = success
        .map([](int x) { return x + 10; })
        .map([](int x) { return x * 3; });
    std::cout << std::format("Chained map result: {}\n", chained.get_value());

    std::cout << std::format("has_value: {}\n", success.has_value() ? "Yes" : "No");
    success.print_compact();
    failure.print_compact();

    std::cout << "\n--- parse_int ---\n";
    for (std::string_view sv : {"42", "hello", "-7", ""}) {
        auto r = parse_int(sv);
        r.print_state();
    }

    std::cout << "\n--- checked_divide ---\n";
    for (auto [a, b] : std::vector<std::pair<int,int>>{{10,2},{10,0},{7,3}}) {
        auto r = checked_divide(a, b);
        std::cout << std::format("{}/{} -> ", a, b);
        r.print_state();
    }

    std::cout << "\n--- map_error ---\n";
    auto upper_err = failure.map_error([](const std::string& e) {
        std::string s = e;
        std::ranges::transform(s, s.begin(), [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        });
        return s;
    });
    upper_err.print_state();

    std::cout << "\n--- std::expected direct ---\n";
    std::expected<int, std::string> exp = 99;
    std::cout << std::format("expected value: {}\n", exp.value());
    exp = std::unexpected("oops");
    std::cout << std::format("expected error: {}\n", exp.error());
    std::cout << std::format("value_or: {}\n", exp.value_or(-1));

    std::cout << "\n--- views pipeline on Result chain ---\n";
    auto pipeline = std::views::iota(1, 6)
        | std::views::transform([](int x) {
            return checked_divide(100, x);
          })
        | std::views::filter([](const auto& r) { return r.success(); })
        | std::views::transform([](const auto& r) { return r.get_value(); });

    std::cout << "100/n results: ";
    for (int v : pipeline) std::cout << std::format("{} ", v);
    std::cout << '\n';

    assert(success.success());
    assert(failure.failure());
    assert(success.value_or(0) == 42);
    assert(failure.value_or(0) == 0);
    assert(doubled.get_value() == 84);
    assert(chained.get_value() == (42 + 10) * 3);
    assert(parse_int("42").get_value() == 42);
    assert(parse_int("bad").failure());
    assert(checked_divide(10, 2).get_value() == 5);
    assert(checked_divide(10, 0).failure());

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
