#include <iostream>
#include <functional>
#include <vector>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <cassert>
#include <utility>
#include <concepts>
#include <optional>
#include <variant>
#include <unordered_map>
#include <string>

[[nodiscard]] int add(int a, int b) noexcept { return a + b; }
[[nodiscard]] int sub(int a, int b) noexcept { return a - b; }

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

class CommandRegistry {
    std::unordered_map<std::string, std::function<int(int, int)>> commands_;

public:
    void register_command(std::string name, std::function<int(int, int)> fn) {
        commands_.emplace(std::move(name), std::move(fn));
    }

    [[nodiscard]] std::optional<int> run(std::string_view name, int a, int b) const {
        auto it = commands_.find(std::string(name));
        if (it == commands_.end()) return std::nullopt;
        return it->second(a, b);
    }

    [[nodiscard]] std::size_t size() const noexcept { return commands_.size(); }
};

template<typename Sig>
class FunctionCache;

template<typename R, typename Arg>
class FunctionCache<R(Arg)> {
    std::function<R(Arg)>      fn_;
    mutable std::optional<Arg> last_arg_;
    mutable std::optional<R>   last_result_;

public:
    explicit FunctionCache(std::function<R(Arg)> fn) : fn_(std::move(fn)) {}

    [[nodiscard]] R operator()(Arg arg) const {
        if (last_arg_ && *last_arg_ == arg) return *last_result_;
        last_arg_    = arg;
        last_result_ = fn_(arg);
        return *last_result_;
    }
};

template<typename... Funcs>
class Overloaded : public Funcs... {
public:
    using Funcs::operator()...;
};

template<typename... Funcs>
Overloaded(Funcs...) -> Overloaded<Funcs...>;

[[nodiscard]] std::function<int(int)> compose(std::function<int(int)> f,
                                               std::function<int(int)> g) {
    return [f = std::move(f), g = std::move(g)](int x) { return f(g(x)); };
}

template<typename T>
[[nodiscard]] std::function<T(T)> identity_fn() {
    return [](T x) { return x; };
}

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

    std::cout << "\n--- Command Registry ---\n";
    CommandRegistry registry;
    registry.register_command("add", add);
    registry.register_command("sub", sub);
    registry.register_command("mul", [](int a, int b) { return a * b; });

    if (auto r = registry.run("add", 4, 5)) std::cout << "add(4,5)=" << *r << '\n';
    if (auto r = registry.run("mul", 4, 5)) std::cout << "mul(4,5)=" << *r << '\n';
    if (!registry.run("div", 4, 5))         std::cout << "div: not registered\n";
    std::cout << "Registered commands: " << registry.size() << '\n';

    std::cout << "\n--- FunctionCache ---\n";
    int call_count = 0;
    FunctionCache<int(int)> cached_square([&](int x) { ++call_count; return x * x; });
    std::cout << "cached_square(7)=" << cached_square(7) << " calls=" << call_count << '\n';
    std::cout << "cached_square(7)=" << cached_square(7) << " calls=" << call_count << " (cached)\n";
    std::cout << "cached_square(9)=" << cached_square(9) << " calls=" << call_count << '\n';

    std::cout << "\n--- Overloaded visitor ---\n";
    auto visitor = Overloaded{
        [](int i)    { std::cout << "int: "    << i << '\n'; },
        [](double d) { std::cout << "double: " << d << '\n'; },
    };
    std::variant<int, double> v1 = 42;
    std::variant<int, double> v2 = 3.14;
    std::visit(visitor, v1);
    std::visit(visitor, v2);

    std::cout << "\n--- compose / identity_fn ---\n";
    auto double_then_inc = compose([](int x) { return x + 1; }, [](int x) { return x * 2; });
    std::cout << "double_then_inc(5)=" << double_then_inc(5) << '\n';

    auto id = identity_fn<int>();
    std::cout << "identity_fn(99)=" << id(99) << '\n';

    assert(execute_operation(add, 2, 3) == 5);
    assert(registry.run("add", 1, 1).value() == 2);
    assert(!registry.run("nope", 1, 1).has_value());
    assert(cached_square(7) == 49);
    assert(call_count == 3);
    assert(double_then_inc(5) == 11);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
