#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <memory>
#include <algorithm>
#include <functional> // 🔹 added
#include <string>     // ✅ ADDED
#include <map>        // ✅ ADDED

// ---------------- ORIGINAL FUNCTION ----------------
auto sum_vector(std::vector<int> vec) {
    return [v = std::move(vec)]() mutable {
        int sum = std::accumulate(v.begin(), v.end(), 0);
        v.clear();
        return sum;
    };
}

// ---------------- EXTRA UTILITIES ----------------

// multiplier factory
auto make_multiplier(int m) {
    return [m](int x) {
        return x * m;
    };
}

// capture-by-reference demo (safe via shared_ptr)
auto capture_by_reference_demo() {
    auto value = std::make_shared<int>(10);

    return [value]() mutable {
        *value += 5;
        return *value;
    };
}

// capture-by-value demo
auto capture_by_value_demo() {
    int value = 20;

    return [value]() mutable {
        value += 10;
        return value;
    };
}

// helper function
int sum_range(int a, int b) {

    int s = 0;

    for (int i = a; i <= b; ++i)
        s += i;

    return s;
}

// ----------- NEW ADDITIONS -----------

// Higher-order function (takes lambda)
void apply_and_print(
    int x,
    const std::function<int(int)>& func
) {
    std::cout << "Applied result: "
              << func(x) << '\n';
}

// Lambda returning lambda (nested)
auto nested_lambda() {

    return [](int a) {

        return [a](int b) {
            return a + b;
        };
    };
}

// Generic lambda with capture
auto make_power(int exp) {

    return [exp](auto base) {

        int result = 1;

        for (int i = 0; i < exp; ++i)
            result *= base;

        return result;
    };
}

// ------------------------------------

// ----------- EXTRA NEW ADDITIONS -----------

// Lambda factory
auto make_subtractor(int value) {

    return [value](int x) {
        return x - value;
    };
}

// Predicate helper
bool check_all_positive(
    const std::vector<int>& values
) {

    return std::all_of(
        values.begin(),
        values.end(),
        [](int n) {
            return n > 0;
        }
    );
}

// Printer utility
auto make_printer(const std::string& prefix) {

    return [prefix](const auto& value) {
        std::cout << prefix
                  << value
                  << '\n';
    };
}

// Store lambdas in container
void lambda_container_demo() {

    std::cout << "\n--- Lambda Container Demo ---\n";

    std::vector<std::function<void()>> tasks;

    tasks.push_back([] {
        std::cout << "Task 1 executed\n";
    });

    tasks.push_back([] {
        std::cout << "Task 2 executed\n";
    });

    tasks.push_back([] {
        std::cout << "Task 3 executed\n";
    });

    for (const auto& task : tasks)
        task();
}

// Mutable capture demo
void mutable_capture_demo() {

    std::cout << "\n--- Mutable Capture Demo ---\n";

    int value = 1;

    auto lambda = [value]() mutable {

        for (int i = 0; i < 3; ++i)
            ++value;

        return value;
    };

    std::cout << "Final mutable value: "
              << lambda() << '\n';
}

// Immediately invoked generic lambda
void iigl_demo() {

    std::cout << "\n--- IIGL Demo ---\n";

    auto result = [](auto a, auto b) {
        return a * b;
    }(6, 7);

    std::cout << "6 * 7 = "
              << result << '\n';
}

// ------------------------------------

// ---------------- MAIN ----------------
int main() {

    std::vector<int> vec = {1, 2, 3, 4, 5};

    int x = [vec](int a, int b) {

        return std::accumulate(
            vec.begin(),
            vec.end(),
            0
        ) * a * b;

    }(1, 1);

    std::cout << "IILE result: "
              << x << '\n';

    auto fn = sum_vector(vec);

    std::cout << "First sum_vector call: "
              << fn() << '\n';

    std::cout << "Second sum_vector call: "
              << fn() << '\n';

    std::cout << "\n--- Extra Lambda Demos ---\n";

    auto times3 = make_multiplier(3);

    std::cout << "3 * 4 = "
              << times3(4) << '\n';

    auto ref_lambda =
        capture_by_reference_demo();

    std::cout << "Ref lambda call 1: "
              << ref_lambda() << '\n';

    std::cout << "Ref lambda call 2: "
              << ref_lambda() << '\n';

    auto val_lambda =
        capture_by_value_demo();

    std::cout << "Val lambda call 1: "
              << val_lambda() << '\n';

    std::cout << "Val lambda call 2: "
              << val_lambda() << '\n';

    auto sum_lambda =
        [](int a, int b) {

        return sum_range(a, b);

    };

    std::cout << "Sum 1..10 = "
              << sum_lambda(1, 10)
              << '\n';

    // ---------------- SMALL ADDITIONS ----------------

    auto add = [](auto a, auto b) {
        return a + b;
    };

    std::cout << "Add 5 + 7 = "
              << add(5, 7) << '\n';

    int counter = 0;

    auto increment = [&counter]() {
        return ++counter;
    };

    std::cout << "Counter: "
              << increment() << " "
              << increment() << " "
              << increment() << '\n';

    auto make_adder = [](int base) {

        return [base](int value) {
            return base + value;
        };
    };

    auto add10 = make_adder(10);

    std::cout << "10 + 5 = "
              << add10(5) << '\n';

    std::vector<int> doubled(vec.size());

    std::transform(
        vec.begin(),
        vec.end(),
        doubled.begin(),

        [](int n) {
            return n * 2;
        }
    );

    std::cout << "Doubled values: ";

    for (int n : doubled)
        std::cout << n << " ";

    std::cout << '\n';

    constexpr auto square =
        [](int n) constexpr {

        return n * n;
    };

    std::cout << "Square of 8 = "
              << square(8) << '\n';

    // -------- NEW FEATURE USAGE --------

    int val = 5;

    auto non_mut = [val]() {
        return val;
    };

    auto mut = [val]() mutable {
        val++;
        return val;
    };

    std::cout << "Mutable: "
              << mut()
              << ", Non-mutable: "
              << non_mut()
              << '\n';

    auto ptr = std::make_unique<int>(100);

    auto move_lambda =
        [p = std::move(ptr)]() {

        return *p;
    };

    std::cout << "Move-only capture: "
              << move_lambda() << '\n';

    std::sort(
        vec.begin(),
        vec.end(),

        [](int a, int b) {
            return a > b;
        }
    );

    std::cout << "Sorted descending: ";

    for (int n : vec)
        std::cout << n << " ";

    std::cout << '\n';

    auto factorial =
        [](auto self, int n) -> int {

        return (n <= 1)
            ? 1
            : n * self(self, n - 1);
    };

    std::cout << "Factorial 5 = "
              << factorial(factorial, 5)
              << '\n';

    apply_and_print(5, times3);

    auto nl = nested_lambda();

    std::cout << "Nested lambda (5 + 3): "
              << nl(5)(3) << '\n';

    auto power2 = make_power(2);

    std::cout << "4^2 = "
              << power2(4) << '\n';

    // ----------- EXTRA SMALL ADDITIONS (NEW) -----------

    // Filter even numbers using lambda
    std::cout << "Even numbers: ";

    for (int n : vec) {

        if ([](int x) {
            return x % 2 == 0;
        }(n))

            std::cout << n << " ";
    }

    std::cout << '\n';

    // Lambda capturing by reference and modifying vector
    auto add_to_all = [&vec](int val) {

        for (auto& n : vec)
            n += val;
    };

    add_to_all(5);

    std::cout << "After adding 5 to all: ";

    for (int n : vec)
        std::cout << n << " ";

    std::cout << '\n';

    // ----------- MORE EXTRA USAGE -----------

    std::cout << "\n--- Subtractor Factory ---\n";

    auto minus5 = make_subtractor(5);

    std::cout << "20 - 5 = "
              << minus5(20) << '\n';

    std::cout << "\n--- all_of Demo ---\n";

    std::cout << "All positive? "
              << (check_all_positive(vec)
                  ? "Yes"
                  : "No")
              << '\n';

    std::cout << "\n--- Printer Factory ---\n";

    auto printer = make_printer("[Value] ");

    printer(123);
    printer(std::string("Lambda"));

    lambda_container_demo();

    mutable_capture_demo();

    iigl_demo();

    // ----------- EVEN MORE ADDITIONS -----------

    std::cout << "\n--- for_each Demo ---\n";

    std::for_each(
        vec.begin(),
        vec.end(),

        [](int n) {
            std::cout
                << "[" << n << "]";
        }
    );

    std::cout << '\n';

    std::cout << "\n--- Lambda as Comparator ---\n";

    std::vector<std::string> words =
        {"apple", "kiwi", "banana", "pear"};

    std::sort(
        words.begin(),
        words.end(),

        [](const std::string& a,
           const std::string& b) {

            return a.size() < b.size();
        }
    );

    for (const auto& word : words)
        std::cout << word << " ";

    std::cout << '\n';

    std::cout << "\n--- Capture Initializer Demo ---\n";

    int base = 50;

    auto capture_init =
        [value = base * 2]() {

        return value;
    };

    std::cout << "Captured initialized value: "
              << capture_init() << '\n';

    // ----------------------------------

    std::cout
        << "Program finished successfully.\n";

    return 0;
}
