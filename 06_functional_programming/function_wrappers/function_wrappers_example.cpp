// Function Wrappers Exercise
// std::function and type erasure

#include <iostream>
#include <functional>
#include <vector>
#include <typeinfo>   // ✅ ADDED
#include <string>     // 🔹 ADDED
#include <algorithm>  // 🔹 ADDED
#include <cassert>    // 🔹 ADDED
#include <utility>    // 🔹 ADDED

int add(int a, int b) {
    return a + b;
}

struct Multiplier {

    int factor;

    int operator()(int x) const {
        return x * factor;
    }
};

struct Calculator {

    int base = 100;

    int increment(int x) const {
        return base + x;
    }
};

// ======================================================
// EXTRA SMALL ADDITIONS
// ======================================================

// Execute binary operation
int execute_operation(
    const std::function<int(int, int)>& op,
    int a,
    int b) {

    return op(a, b);
}

// Print callable information
template<typename Func>
void callable_info(const Func&) {

    std::cout << "Callable type: "
              << typeid(Func).name()
              << "\n";
}

// Simple event system
class EventDispatcher {

private:
    std::vector<std::function<void()>> listeners;

public:
    void subscribe(std::function<void()> fn) {
        listeners.push_back(std::move(fn));
    }

    void notify() const {

        for (const auto& fn : listeners) {
            fn();
        }
    }

    std::size_t listener_count() const {
        return listeners.size();
    }
};

// Function pipeline
int apply_pipeline(
    int value,
    const std::vector<std::function<int(int)>>& ops) {

    for (const auto& op : ops) {
        value = op(value);
    }

    return value;
}

// ======================================================
// Main
// ======================================================
int main() {

    std::vector<std::function<int()>> functions;

    int value = 10;

    functions.push_back([value]() {
        return value + 5;
    });

    functions.push_back([value]() {
        return value * 2;
    });

    functions.push_back(
        [value, multiplier = Multiplier{3}]() {
            return multiplier(value);
        }
    );

    functions.push_back([counter = 0]() mutable {
        return ++counter;
    });

    for (const auto& f : functions) {

        std::cout << "Result: "
                  << f() << "\n";
    }

    std::cout << "\n--- Direct std::function usage ---\n";

    std::function<int(int, int)> func = add;

    std::cout << "add(3,4): "
              << func(3,4) << "\n";

    func = [](int a, int b) {
        return a * b;
    };

    std::cout << "multiply(3,4): "
              << func(3,4) << "\n";

    auto add_10 =
        std::bind(add,
                  std::placeholders::_1,
                  10);

    std::cout << "Bound result: "
              << add_10(5) << "\n";

    std::cout << "\n--- Member function binding ---\n";

    Calculator calc;

    std::function<int(int)> memberFunc =
        std::bind(
            &Calculator::increment,
            calc,
            std::placeholders::_1
        );

    std::cout << "Member function result: "
              << memberFunc(50) << "\n";

    std::cout << "\n--- std::invoke example ---\n";

    std::cout << "Invoke add: "
              << std::invoke(add, 7, 8)
              << "\n";

    std::cout << "Invoke functor: "
              << std::invoke(Multiplier{5}, 6)
              << "\n";

    std::cout << "\n--- Simple callback dispatcher ---\n";

    std::vector<std::function<void()>> callbacks;

    callbacks.push_back([]() {
        std::cout << "Callback A\n";
    });

    callbacks.push_back([]() {
        std::cout << "Callback B\n";
    });

    for (auto& cb : callbacks) {
        cb();
    }

    // ---------------- SMALL ADDITIONS ----------------

    std::cout << "\n--- Extra Tests ---\n";

    std::function<int(int)> square =
        [](int x) {
            return x * x;
        };

    std::cout << "Square(6): "
              << square(6) << "\n";

    int base = 5;

    std::function<int(int)> add_base =
        [base](int x) {
            return x + base;
        };

    std::cout << "Add base (5 + 10): "
              << add_base(10) << "\n";

    std::function<void()> printer =
        []() {
            std::cout << "First function\n";
        };

    printer();

    printer = []() {
        std::cout << "Reassigned function\n";
    };

    printer();

    std::vector<std::function<int(int)>> operations;

    operations.push_back([](int x) {
        return x + 1;
    });

    operations.push_back([](int x) {
        return x * 2;
    });

    operations.push_back([](int x) {
        return x - 3;
    });

    std::cout << "Operations on 10:\n";

    for (auto& op : operations) {
        std::cout << op(10) << " ";
    }

    std::cout << "\n";

    std::cout << "Invoke member directly: "
              << std::invoke(
                     &Calculator::increment,
                     calc,
                     25)
              << "\n";

    // -------- NEW SMALL ADDITIONS --------

    // Check if function is empty
    std::function<void()> empty_func;

    if (!empty_func)
        std::cout << "Function is empty\n";

    // Target type inspection
    std::cout << "Target type of func: "
              << func.target_type().name()
              << "\n";

    // Chained execution
    std::function<int(int)> op_chain =
        [](int x) {
            return x + 2;
        };

    op_chain = [op_chain](int x) {
        return op_chain(x) * 3;
    };

    std::cout << "Chained op (5): "
              << op_chain(5) << "\n";

    // Store and execute mixed callable returning void
    std::vector<std::function<void(int)>> printers;

    printers.push_back([](int x) {
        std::cout << "Val: " << x << "\n";
    });

    printers.push_back([](int x) {
        std::cout << "Square: "
                  << x * x << "\n";
    });

    for (auto& p : printers) {
        p(4);
    }

    // ======================================================
    // EXTRA SMALL USAGE
    // ======================================================

    std::cout << "\n--- Advanced Wrapper Utilities ---\n";

    // Execute operation helper
    std::cout << "execute_operation(add): "
              << execute_operation(add, 7, 3)
              << "\n";

    // Callable info
    callable_info(add);

    auto lambda = [](int x) {
        return x * 10;
    };

    callable_info(lambda);

    // Event dispatcher demo
    EventDispatcher dispatcher;

    dispatcher.subscribe([]() {
        std::cout << "Listener 1 triggered\n";
    });

    dispatcher.subscribe([]() {
        std::cout << "Listener 2 triggered\n";
    });

    std::cout << "Listener count: "
              << dispatcher.listener_count()
              << "\n";

    dispatcher.notify();

    // Function pipeline
    std::vector<std::function<int(int)>> pipeline;

    pipeline.push_back([](int x) {
        return x + 5;
    });

    pipeline.push_back([](int x) {
        return x * 2;
    });

    pipeline.push_back([](int x) {
        return x - 1;
    });

    std::cout << "Pipeline result: "
              << apply_pipeline(10, pipeline)
              << "\n";

    // Sorting with std::function comparator
    std::vector<int> nums = {5, 1, 4, 2, 3};

    std::function<bool(int, int)> comp =
        [](int a, int b) {
            return a > b;
        };

    std::sort(nums.begin(), nums.end(), comp);

    std::cout << "Sorted descending: ";

    for (int n : nums) {
        std::cout << n << " ";
    }

    std::cout << "\n";

    // ✅ Runtime validation
    assert(execute_operation(add, 2, 3) == 5);

    // ======================================================

    return 0;
}
