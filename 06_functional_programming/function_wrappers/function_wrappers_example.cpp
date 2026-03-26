// Function Wrappers Exercise
// std::function and type erasure

#include <iostream>
#include <functional>
#include <vector>

int add(int a, int b) { return a + b; }

struct Multiplier {
    int factor;
    int operator()(int x) const { return x * factor; }
};

struct Calculator {
    int base = 100;
    int increment(int x) const { return base + x; }
};

int main() {

    // std::function can hold any callable
    std::vector<std::function<int()>> functions;
    
    int value = 10;

    // Lambda
    functions.push_back([value]() { return value + 5; });
    functions.push_back([value]() { return value * 2; });

    // Functor
    functions.push_back([value, multiplier = Multiplier{3}]() { 
        return multiplier(value); 
    });

    // Mutable lambda
    functions.push_back([counter = 0]() mutable {
        return ++counter;
    });

    // Execute stored callables
    for (const auto& f : functions) {
        std::cout << "Result: " << f() << "\n";
    }

    std::cout << "\n--- Direct std::function usage ---\n";

    // Store free function
    std::function<int(int, int)> func = add;
    std::cout << "add(3,4): " << func(3,4) << "\n";

    // Reassign to lambda
    func = [](int a, int b) { return a * b; };
    std::cout << "multiply(3,4): " << func(3,4) << "\n";

    // std::bind
    auto add_10 = std::bind(add, std::placeholders::_1, 10);
    std::cout << "Bound result: " << add_10(5) << "\n";

    std::cout << "\n--- Member function binding ---\n";

    Calculator calc;
    std::function<int(int)> memberFunc =
        std::bind(&Calculator::increment, calc, std::placeholders::_1);

    std::cout << "Member function result: " << memberFunc(50) << "\n";

    std::cout << "\n--- std::invoke example ---\n";

    std::cout << "Invoke add: "
              << std::invoke(add, 7, 8) << "\n";

    std::cout << "Invoke functor: "
              << std::invoke(Multiplier{5}, 6) << "\n";

    std::cout << "\n--- Simple callback dispatcher ---\n";

    std::vector<std::function<void()>> callbacks;
    callbacks.push_back([](){ std::cout << "Callback A\n"; });
    callbacks.push_back([](){ std::cout << "Callback B\n"; });

    for (auto& cb : callbacks) {
        cb();
    }

    // ---------------- SMALL ADDITIONS ----------------

    std::cout << "\n--- Extra Tests ---\n";

    // Store lambda returning different computation
    std::function<int(int)> square = [](int x) { return x * x; };
    std::cout << "Square(6): " << square(6) << "\n";

    // Using std::function with capturing lambda
    int base = 5;
    std::function<int(int)> add_base = [base](int x) {
        return x + base;
    };
    std::cout << "Add base (5 + 10): " << add_base(10) << "\n";

    // Function wrapper reassignment
    std::function<void()> printer = []() {
        std::cout << "First function\n";
    };
    printer();

    printer = []() {
        std::cout << "Reassigned function\n";
    };
    printer();

    // Store multiple callables with arguments
    std::vector<std::function<int(int)>> operations;
    operations.push_back([](int x){ return x + 1; });
    operations.push_back([](int x){ return x * 2; });
    operations.push_back([](int x){ return x - 3; });

    std::cout << "Operations on 10:\n";
    for (auto& op : operations) {
        std::cout << op(10) << " ";
    }
    std::cout << "\n";

    // Using std::invoke with member function pointer directly
    std::cout << "Invoke member directly: "
              << std::invoke(&Calculator::increment, calc, 25) << "\n";

    // ------------------------------------------------

    return 0;
}
