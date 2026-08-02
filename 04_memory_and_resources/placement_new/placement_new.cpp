#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <memory>
#include <algorithm>
#include <functional>
#include <string>
#include <string_view>
#include <ranges>
#include <span>
#include <map>
#include <optional>
#include <format>
#include <cassert>

[[nodiscard]] auto sum_vector(std::vector<int> vec) {
    return [v = std::move(vec)]() mutable {
        const int sum = std::reduce(v.begin(), v.end(), 0);
        v.clear();
        return sum;
    };
}

[[nodiscard]] auto make_multiplier(int m) noexcept {
    return [m](int x) noexcept { return x * m; };
}

[[nodiscard]] auto capture_shared_state_demo() {
    auto value = std::make_shared<int>(10);
    return [value]() mutable {
        *value += 5;
        return *value;
    };
}

[[nodiscard]] auto capture_by_value_demo() {
    int value = 20;
    return [value]() mutable {
        value += 10;
        return value;
    };
}

[[nodiscard]] constexpr int sum_range(int a, int b) noexcept {
    int s = 0;
    for (int i = a; i <= b; ++i) s += i;
    return s;
}

void apply_and_print(int x, const std::function<int(int)>& func) {
    std::cout << std::format("Applied result: {}\n", func(x));
}

[[nodiscard]] auto nested_lambda() {
    return [](int a) {
        return [a](int b) { return a + b; };
    };
}

[[nodiscard]] auto make_power(int exp) {
    return [exp](auto base) {
        int result = 1;
        for (int i = 0; i < exp; ++i) result *= base;
        return result;
    };
}

[[nodiscard]] auto make_subtractor(int value) noexcept {
    return [value](int x) noexcept { return x - value; };
}

[[nodiscard]] bool check_all_positive(std::span<const int> values) {
    return std::ranges::all_of(values, [](int n) { return n > 0; });
}

[[nodiscard]] auto make_printer(std::string_view prefix) {
    return [p = std::string(prefix)](const auto& value) {
        std::cout << p << value << '\n';
    };
}

[[nodiscard]] auto make_memoized(std::function<int(int)> fn) {
    return [fn, cache = std::map<int, int>{}](int x) mutable -> int {
        if (auto it = cache.find(x); it != cache.end()) return it->second;
        return cache[x] = fn(x);
    };
}

void with_retry(int attempts, std::invocable<int> auto fn) {
    for (int i = 0; i < attempts; ++i)
        if (fn(i)) return;
    std::cout << std::format("All {} attempts exhausted\n", attempts);
}

template <std::invocable Producer>
[[nodiscard]] auto lazy(Producer p) {
    using T = std::invoke_result_t<Producer>;
    return [p = std::move(p), cache = std::optional<T>{}]() mutable -> const T& {
        if (!cache) cache = std::invoke(p);
        return *cache;
    };
}

[[nodiscard]] auto make_once(std::invocable auto fn) {
    return [fn = std::move(fn), fired = false]() mutable {
        if (!fired) { fired = true; std::invoke(fn); }
        else         std::cout << "(already called — no-op)\n";
    };
}

template <typename F, typename G>
[[nodiscard]] auto compose(F f, G g) {
    return [f = std::move(f), g = std::move(g)](auto&& x) {
        return f(g(std::forward<decltype(x)>(x)));
    };
}

template <typename F, typename G>
[[nodiscard]] auto pipe(F f, G g) {
    return compose(std::move(g), std::move(f));
}

[[nodiscard]] auto make_counted(std::function<void()> fn) {
    return [fn = std::move(fn), count = 0]() mutable {
        ++count;
        std::cout << std::format("  [invocation #{}] ", count);
        fn();
    };
}

void lambda_container_demo() {
    std::cout << "\n--- Lambda Container Demo ---\n";
    std::vector<std::function<void()>> tasks;
    tasks.reserve(3);
    tasks.emplace_back([] { std::cout << "Task 1 executed\n"; });
    tasks.emplace_back([] { std::cout << "Task 2 executed\n"; });
    tasks.emplace_back([] { std::cout << "Task 3 executed\n"; });
    for (const auto& task : tasks) task();
}

void mutable_capture_demo() {
    std::cout << "\n--- Mutable Capture Demo ---\n";
    int value = 1;
    auto lambda = [value]() mutable {
        for (int i = 0; i < 3; ++i) ++value;
        return value;
    };
    std::cout << std::format("Final mutable value: {}\n", lambda());
}

void immediately_invoked_generic_lambda_demo() {
    std::cout << "\n--- Immediately Invoked Generic Lambda Demo ---\n";
    const auto result = [](auto a, auto b) { return a * b; }(6, 7);
    std::cout << std::format("6 * 7 = {}\n", result);
}

int main() {
    std::vector<int> vec{1, 2, 3, 4, 5};

    const int x = [&vec]() {
        return std::reduce(vec.begin(), vec.end(), 0);
    }();
    std::cout << std::format("IILE result: {}\n", x);

    auto fn = sum_vector(vec);
    std::cout << std::format("First sum_vector call: {}\n",  fn());
    std::cout << std::format("Second sum_vector call: {}\n", fn());

    std::cout << "\n--- Extra Lambda Demos ---\n";

    auto times3 = make_multiplier(3);
    std::cout << std::format("3 * 4 = {}\n", times3(4));

    auto shared_lambda = capture_shared_state_demo();
    std::cout << std::format("Shared-state lambda call 1: {}\n", shared_lambda());
    std::cout << std::format("Shared-state lambda call 2: {}\n", shared_lambda());

    auto val_lambda = capture_by_value_demo();
    std::cout << std::format("Val lambda call 1: {}\n", val_lambda());
    std::cout << std::format("Val lambda call 2: {}\n", val_lambda());

    auto sum_lambda = [](int a, int b) { return sum_range(a, b); };
    std::cout << std::format("Sum 1..10 = {}\n", sum_lambda(1, 10));

    auto add = [](auto a, auto b) { return a + b; };
    std::cout << std::format("Add 5 + 7 = {}\n", add(5, 7));

    int counter = 0;
    auto increment = [&counter]() { return ++counter; };
    std::cout << std::format("Counter: {} {} {}\n", increment(), increment(), increment());

    auto make_adder = [](int base_val) {
        return [base_val](int value) { return base_val + value; };
    };
    auto add10 = make_adder(10);
    std::cout << std::format("10 + 5 = {}\n", add10(5));

    std::vector<int> doubled(vec.size());
    std::ranges::transform(vec, doubled.begin(), [](int n) { return n * 2; });
    std::cout << "Doubled values: ";
    for (const int n : doubled) std::cout << n << ' ';
    std::cout << '\n';

    constexpr auto square = [](int n) constexpr { return n * n; };
    std::cout << std::format("Square of 8 = {}\n", square(8));

    int val = 5;
    auto non_mut = [val]()         { return val;   };
    auto mut     = [val]() mutable { return ++val; };
    std::cout << std::format("Mutable: {}, Non-mutable: {}\n", mut(), non_mut());

    auto uptr = std::make_unique<int>(100);
    auto move_lambda = [p = std::move(uptr)]() { return *p; };
    std::cout << std::format("Move-only capture: {}\n", move_lambda());

    std::ranges::sort(vec, std::greater<>{});
    std::cout << "Sorted descending: ";
    for (const int n : vec) std::cout << n << ' ';
    std::cout << '\n';

    auto factorial = [](auto self, int n) -> int {
        return (n <= 1) ? 1 : n * self(self, n - 1);
    };
    std::cout << std::format("Factorial 5 = {}\n", factorial(factorial, 5));

    apply_and_print(5, times3);

    auto nl = nested_lambda();
    std::cout << std::format("Nested lambda (5 + 3): {}\n", nl(5)(3));

    auto power2 = make_power(2);
    std::cout << std::format("4^2 = {}\n", power2(4));

    std::cout << "Even numbers: ";
    for (const int n : vec | std::views::filter([](int v) { return v % 2 == 0; }))
        std::cout << n << ' ';
    std::cout << '\n';

    auto add_to_all = [&vec](int amount) {
        std::ranges::for_each(vec, [amount](int& n) { n += amount; });
    };
    add_to_all(5);
    std::cout << "After adding 5 to all: ";
    for (const int n : vec) std::cout << n << ' ';
    std::cout << '\n';

    std::cout << "\n--- Subtractor Factory ---\n";
    auto minus5 = make_subtractor(5);
    std::cout << std::format("20 - 5 = {}\n", minus5(20));

    std::cout << "\n--- all_of Demo ---\n";
    std::cout << std::format("All positive? {}\n", check_all_positive(vec) ? "Yes" : "No");

    std::cout << "\n--- Printer Factory ---\n";
    auto printer = make_printer("[Value] ");
    printer(123);
    printer(std::string("Lambda"));

    lambda_container_demo();
    mutable_capture_demo();
    immediately_invoked_generic_lambda_demo();

    std::cout << "\n--- for_each Demo ---\n";
    std::ranges::for_each(vec, [](int n) { std::cout << '[' << n << ']'; });
    std::cout << '\n';

    std::cout << "\n--- Lambda as Comparator ---\n";
    std::vector<std::string> words{"apple", "kiwi", "banana", "pear"};
    std::ranges::sort(words, [](std::string_view a, std::string_view b) {
        return a.size() < b.size();
    });
    for (const auto& word : words) std::cout << word << ' ';
    std::cout << '\n';

    std::cout << "\n--- Capture Initializer Demo ---\n";
    const int base = 50;
    auto capture_init = [value = base * 2]() { return value; };
    std::cout << std::format("Captured initialized value: {}\n", capture_init());

    std::cout << "\n--- Memoized Lambda ---\n";
    auto slow_square = make_memoized([](int n) { return n * n; });
    std::cout << std::format("slow_square(7) = {}\n", slow_square(7));
    std::cout << std::format("slow_square(7) = {} (cached)\n", slow_square(7));
    std::cout << std::format("slow_square(9) = {}\n", slow_square(9));

    std::cout << "\n--- Retry Combinator ---\n";
    int attempt_count = 0;
    with_retry(5, [&](int i) -> bool {
        ++attempt_count;
        std::cout << std::format("Attempt {}", i + 1);
        if (i == 2) { std::cout << " -> success\n"; return true; }
        std::cout << " -> fail\n";
        return false;
    });
    std::cout << std::format("Total attempts made: {}\n", attempt_count);

    std::cout << "\n--- Lazy Evaluation ---\n";
    int compute_count = 0;
    auto expensive = lazy([&] {
        ++compute_count;
        std::cout << "  (computing...)\n";
        return 42 * 42;
    });
    std::cout << std::format("Before first access, compute_count = {}\n", compute_count);
    std::cout << std::format("lazy value = {}\n", expensive());
    std::cout << std::format("lazy value = {} (no recompute)\n", expensive());
    std::cout << std::format("compute_count = {}\n", compute_count);

    std::cout << "\n--- Transform + Accumulate Pipeline ---\n";
    const std::vector<int> nums{1, 2, 3, 4, 5};
    const auto pipeline_result = std::accumulate(nums.begin(), nums.end(), 0,
        [sq = square](int acc, int n) { return acc + sq(n); });
    std::cout << std::format("Sum of squares(1..5) = {}\n", pipeline_result);

    std::cout << "\n--- once combinator ---\n";
    auto greet_once = make_once([] { std::cout << "Hello! (only once)\n"; });
    greet_once();
    greet_once();
    greet_once();

    std::cout << "\n--- compose / pipe ---\n";
    auto double_then_inc = compose([](int n) { return n + 1; },
                                   [](int n) { return n * 2; });
    auto inc_then_double = pipe([](int n) { return n + 1; },
                                [](int n) { return n * 2; });
    std::cout << std::format("compose(+1, *2)(5) = {} (expect 11)\n", double_then_inc(5));
    std::cout << std::format("pipe(+1, *2)(5)    = {} (expect 12)\n", inc_then_double(5));
    assert(double_then_inc(5) == 11);
    assert(inc_then_double(5) == 12);

    std::cout << "\n--- counted invocation ---\n";
    auto counted_task = make_counted([] { std::cout << "task body\n"; });
    counted_task();
    counted_task();
    counted_task();

    std::cout << "\n--- transform_reduce with lambdas ---\n";
    const std::vector<int> weights{1, 2, 3, 4, 5};
    const std::vector<int> values2{10, 20, 30, 40, 50};
    const int dot = std::transform_reduce(
        weights.begin(), weights.end(),
        values2.begin(),
        0,
        std::plus<int>{},
        [](int w, int v) { return w * v; });
    std::cout << std::format("dot product = {} (expect 550)\n", dot);
    assert(dot == 550);

    std::cout << "\nProgram finished successfully.\n";
    return 0;
}
