#include <iostream>
#include <vector>
#include <numeric>
#include <utility>
#include <memory>
#include <algorithm>   // ✅ small required addition

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

// ---------------- MAIN ----------------
int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // IILE (Immediately Invoked Lambda Expression)
    int x = [vec](int a, int b) {
        return std::accumulate(vec.begin(), vec.end(), 0) * a * b;
    }(1, 1);

    std::cout << "IILE result: " << x << '\n';

    auto fn = sum_vector(vec);
    std::cout << "First sum_vector call: " << fn() << '\n';
    std::cout << "Second sum_vector call: " << fn() << '\n';

    std::cout << "\n--- Extra Lambda Demos ---\n";

    auto times3 = make_multiplier(3);
    std::cout << "3 * 4 = " << times3(4) << '\n';

    auto ref_lambda = capture_by_reference_demo();
    std::cout << "Ref lambda call 1: " << ref_lambda() << '\n';
    std::cout << "Ref lambda call 2: " << ref_lambda() << '\n';

    auto val_lambda = capture_by_value_demo();
    std::cout << "Val lambda call 1: " << val_lambda() << '\n';
    std::cout << "Val lambda call 2: " << val_lambda() << '\n';

    auto sum_lambda = [](int a, int b) {
        return sum_range(a, b);
    };
    std::cout << "Sum 1..10 = " << sum_lambda(1, 10) << '\n';

    // ---------------- SMALL ADDITIONS ----------------

    // square lambda
    auto square = [](int n) { return n * n; };
    std::cout << "Square of 6 = " << square(6) << '\n';

    // even check lambda
    auto is_even = [](int n) { return n % 2 == 0; };
    std::cout << "Is 10 even? " << (is_even(10) ? "Yes" : "No") << '\n';

    // max of two numbers lambda
    auto max_of_two = [](int a, int b) {
        return (a > b) ? a : b;
    };
    std::cout << "Max of 7 and 3 = " << max_of_two(7, 3) << '\n';

    // count even numbers in vector
    auto count_even = [](const std::vector<int>& v) {
        return std::count_if(v.begin(), v.end(),
                             [](int n) { return n % 2 == 0; });
    };
    std::cout << "Even numbers count = " << count_even(vec) << '\n';

    // stateful mutable lambda (running counter)
    auto running_counter = [count = 0]() mutable {
        return ++count;
    };
    std::cout << "Counter calls: "
              << running_counter() << " "
              << running_counter() << " "
              << running_counter() << '\n';

    // constant lambda
    auto say_done = []() {
        std::cout << "Program finished successfully.\n";
    };
    say_done();

    return 0;
}
 
