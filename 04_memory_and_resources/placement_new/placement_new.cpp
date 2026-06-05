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

[[nodiscard]] auto sum_vector(std::vector<int> vec) {
    return [v = std::move(vec)]() mutable {
        int sum = std::accumulate(v.begin(), v.end(), 0);
        v.clear();
        return sum;
    };
}

[[nodiscard]] auto make_multiplier(int m) {
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
    std::cout << "Applied result: " << func(x) << '\n';
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

[[nodiscard]] auto make_subtractor(int value) {
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
    std::cout << "Final mutable value: " << lambda() << '\n';
}

void immediately_invoked_generic_lambda_demo() {
    std::cout << "\n--- Immediately Invoked Generic Lambda Demo ---\n";
    auto result = [](auto a, auto b) { return a * b; }(6, 7);
    std::cout << "6 * 7 = " << result << '\n';
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    int x = [&vec]() {
        return std::accumulate(vec.begin(), vec.end(), 0);
    }();
    std::cout << "IILE result: " << x << '\n';

    auto fn = sum_vector(vec);
    std::cout << "First sum_vector call: "  << fn() << '\n';
    std::cout << "Second sum_vector call: " << fn() << '\n';

    std::cout << "\n--- Extra Lambda Demos ---\n";

    auto times3 = make_multiplier(3);
    std::cout << "3 * 4 = " << times3(4) << '\n';

    auto shared_lambda = capture_shared_state_demo();
    std::cout << "Shared-state lambda call 1: " << shared_lambda() << '\n';
    std::cout << "Shared-state lambda call 2: " << shared_lambda() << '\n';

    auto val_lambda = capture_by_value_demo();
    std::cout << "Val lambda call 1: " << val_lambda() << '\n';
    std::cout << "Val lambda call 2: " << val_lambda() << '\n';

    auto sum_lambda = [](int a, int b) { return sum_range(a, b); };
    std::cout << "Sum 1..10 = " << sum_lambda(1, 10) << '\n';

    auto add = [](auto a, auto b) { return a + b; };
    std::cout << "Add 5 + 7 = " << add(5, 7) << '\n';

    int counter = 0;
    auto increment = [&counter]() { return ++counter; };
    std::cout << "Counter: " << increment() << ' ' << increment() << ' ' << increment() << '\n';

    auto make_adder = [](int base) {
        return [base](int value) { return base + value; };
    };
    auto add10 = make_adder(10);
    std::cout << "10 + 5 = " << add10(5) << '\n';

    std::vector<int> doubled(vec.size());
    std::ranges::transform(vec, doubled.begin(), [](int n) { return n * 2; });
    std::cout << "Doubled values: ";
    for (int n : doubled) std::cout << n << ' ';
    std::cout << '\n';

    constexpr auto square = [](int n) constexpr { return n * n; };
    std::cout << "Square of 8 = " << square(8) << '\n';

    int val = 5;
    auto non_mut = [val]()         { return val;    };
    auto mut     = [val]() mutable { return ++val;  };
    std::cout << "Mutable: " << mut() << ", Non-mutable: " << non_mut() << '\n';

    auto uptr = std::make_unique<int>(100);
    auto move_lambda = [p = std::move(uptr)]() { return *p; };
    std::cout << "Move-only capture: " << move_lambda() << '\n';

    std::ranges::sort(vec, std::greater<>{});
    std::cout << "Sorted descending: ";
    for (int n : vec) std::cout << n << ' ';
    std::cout << '\n';

    auto factorial = [](auto self, int n) -> int {
        return (n <= 1) ? 1 : n * self(self, n - 1);
    };
    std::cout << "Factorial 5 = " << factorial(factorial, 5) << '\n';

    apply_and_print(5, times3);

    auto nl = nested_lambda();
    std::cout << "Nested lambda (5 + 3): " << nl(5)(3) << '\n';

    auto power2 = make_power(2);
    std::cout << "4^2 = " << power2(4) << '\n';

    std::cout << "Even numbers: ";
    for (int n : vec | std::views::filter([](int x) { return x % 2 == 0; })) {
        std::cout << n << ' ';
    }
    std::cout << '\n';

    auto add_to_all = [&vec](int amount) {
        std::ranges::for_each(vec, [amount](int& n) { n += amount; });
    };
    add_to_all(5);
    std::cout << "After adding 5 to all: ";
    for (int n : vec) std::cout << n << ' ';
    std::cout << '\n';

    std::cout << "\n--- Subtractor Factory ---\n";
    auto minus5 = make_subtractor(5);
    std::cout << "20 - 5 = " << minus5(20) << '\n';

    std::cout << "\n--- all_of Demo ---\n";
    std::cout << "All positive? " << (check_all_positive(vec) ? "Yes" : "No") << '\n';

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
    std::vector<std::string> words = {"apple", "kiwi", "banana", "pear"};
    std::ranges::sort(words, [](std::string_view a, std::string_view b) {
        return a.size() < b.size();
    });
    for (const auto& word : words) std::cout << word << ' ';
    std::cout << '\n';

    std::cout << "\n--- Capture Initializer Demo ---\n";
    int base = 50;
    auto capture_init = [value = base * 2]() { return value; };
    std::cout << "Captured initialized value: " << capture_init() << '\n';

    std::cout << "Program finished successfully.\n";
    return 0;
}
