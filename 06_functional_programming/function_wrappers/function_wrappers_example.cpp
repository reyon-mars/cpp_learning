#include <iostream>
#include <functional>
#include <vector>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <cassert>
#include <utility>
#include <concepts>

[[nodiscard]] int add(int a, int b) noexcept { return a + b; }

struct Multiplier {
    int factor;
    [[nodiscard]] int operator()(int x) const noexcept { return x * factor; }
};

struct Calculator {
    int base = 100;
    [[nodiscard]] int increment(int x) const noexcept { return base + x; }
};

template<std::invocable<int, int> BinaryOp>
[[nodiscard]] int execute_operation(BinaryOp&& op, int a, int b) {
    return std::invoke(std::forward<BinaryOp>(op), a, b);
}

[[nodiscard]] int apply_pipeline(int value,
                                  std::span<const std::function<int(int)>> ops) {
    return std::accumulate(ops.begin(), ops.end(), value,
                           [](int v, const auto& op) { return op(v); });
}

class EventDispatcher {
    std::vector<std::function<void()>> listeners_;

public:
    void subscribe(std::function<void()> fn) {
        listeners_.push_back(std::move(fn));
    }

    void notify() const {
        std::ranges::for_each(listeners_, [](const auto& fn) { fn(); });
    }

    [[nodiscard]] std::size_t listener_count() const noexcept {
        return listeners_.size();
    }
};

int main() {
    std::vector<std::function<int()>> functions;
    functions.reserve(4);

    const int value = 10;
    functions.emplace_back([value]()                               { return value + 5;          });
    functions.emplace_back([value]()                               { return value * 2;          });
    functions.emplace_back([value, m = Multiplier{3}]()           { return m(value);            });
    functions.emplace_back([counter = 0]() mutable                { return ++counter;           });

    for (const auto& f : functions)
        std::cout << "Result: " << f() << '\n';

    std::cout << "\n--- Direct std::function usage ---\n";
    std::function<int(int, int)> func = add;
    std::cout << "add(3,4):      " << func(3, 4) << '\n';

    func = [](int a, int b) { return a * b; };
    std::cout << "multiply(3,4): " << func(3, 4) << '\n';

    auto add_10 = std::bind_front(add, 10);
    std::cout << "Bound result:  " << add_10(5) << '\n';

    std::cout << "\n--- Member function binding ---\n";
    Calculator calc;
    auto memberFunc = std::bind_front(&Calculator::increment, calc);
    std::cout << "Member function result: " << memberFunc(50) << '\n';

    std::cout << "\n--- std::invoke ---\n";
    std::cout << "Invoke add:     " << std::invoke(add, 7, 8)          << '\n';
    std::cout << "Invoke functor: " << std::invoke(Multiplier{5}, 6)   << '\n';
    std::cout << "Invoke member:  " << std::invoke(&Calculator::increment, calc, 25) << '\n';

    std::cout << "\n--- Callback dispatcher ---\n";
    std::vector<std::function<void()>> callbacks;
    callbacks.reserve(2);
    callbacks.emplace_back([] { std::cout << "Callback A\n"; });
    callbacks.emplace_back([] { std::cout << "Callback B\n"; });
    std::ranges::for_each(callbacks, [](const auto& cb) { cb(); });

    std::cout << "\n--- Composed and reassigned functions ---\n";
    std::function<int(int)> square = [](int x) { return x * x; };
    std::cout << "square(6): " << square(6) << '\n';

    const int base = 5;
    std::function<int(int)> add_base = [base](int x) { return x + base; };
    std::cout << "add_base(10): " << add_base(10) << '\n';

    std::function<void()> printer = [] { std::cout << "First function\n";      };
    printer();
    printer =                       [] { std::cout << "Reassigned function\n"; };
    printer();

    std::cout << "\n--- Operations on 10 ---\n";
    const std::vector<std::function<int(int)>> operations = {
        [](int x) { return x + 1; },
        [](int x) { return x * 2; },
        [](int x) { return x - 3; },
    };
    for (const auto& op : operations)
        std::cout << op(10) << ' ';
    std::cout << '\n';

    std::cout << "\n--- Empty function check ---\n";
    std::function<void()> empty_func;
    if (!empty_func)
        std::cout << "Function is empty\n";

    std::cout << "\n--- Chained operations ---\n";
    std::function<int(int)> op_chain = [](int x) { return x + 2; };
    op_chain = [inner = std::move(op_chain)](int x) { return inner(x) * 3; };
    std::cout << "Chained op(5): " << op_chain(5) << '\n';

    std::cout << "\n--- Mixed void(int) callable vector ---\n";
    const std::vector<std::function<void(int)>> printers = {
        [](int x) { std::cout << "Val:    " << x       << '\n'; },
        [](int x) { std::cout << "Square: " << x * x   << '\n'; },
    };
    for (const auto& p : printers) p(4);

    std::cout << "\n--- Advanced Wrapper Utilities ---\n";
    std::cout << "execute_operation(add, 7, 3): " << execute_operation(add, 7, 3) << '\n';

    EventDispatcher dispatcher;
    dispatcher.subscribe([] { std::cout << "Listener 1 triggered\n"; });
    dispatcher.subscribe([] { std::cout << "Listener 2 triggered\n"; });
    std::cout << "Listener count: " << dispatcher.listener_count() << '\n';
    dispatcher.notify();

    const std::vector<std::function<int(int)>> pipeline = {
        [](int x) { return x + 5; },
        [](int x) { return x * 2; },
        [](int x) { return x - 1; },
    };
    std::cout << "Pipeline result: " << apply_pipeline(10, pipeline) << '\n';

    std::cout << "\n--- Sorted descending ---\n";
    std::vector<int> nums = {5, 1, 4, 2, 3};
    std::ranges::sort(nums, std::greater<>{});
    for (int n : nums) std::cout << n << ' ';
    std::cout << '\n';

    assert(execute_operation(add, 2, 3) == 5);
    std::cout << "\nAll assertions passed.\n";

    return 0;
}
