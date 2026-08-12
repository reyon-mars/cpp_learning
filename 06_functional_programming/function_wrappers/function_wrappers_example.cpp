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
#include <format>
#include <map>
#include <type_traits>

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

template <std::invocable<int, int> BinaryOp>
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
    void subscribe(std::function<void()> fn) { listeners_.push_back(std::move(fn)); }
    void notify() const { std::ranges::for_each(listeners_, [](const auto& fn) { fn(); }); }
    [[nodiscard]] std::size_t listener_count() const noexcept { return listeners_.size(); }
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

template <typename Sig>
class FunctionCache;

template <typename R, typename Arg>
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

template <typename... Funcs>
class Overloaded : public Funcs... { public: using Funcs::operator()...; };
template <typename... Funcs>
Overloaded(Funcs...) -> Overloaded<Funcs...>;

[[nodiscard]] std::function<int(int)> compose(std::function<int(int)> f,
                                               std::function<int(int)> g) {
    return [f = std::move(f), g = std::move(g)](int x) { return f(g(x)); };
}

template <typename T>
[[nodiscard]] std::function<T(T)> identity_fn() {
    return [](T x) { return x; };
}

template <std::invocable<int> Fn>
[[nodiscard]] auto memoize(Fn fn) {
    return [fn = std::move(fn),
            cache = std::map<int, std::invoke_result_t<Fn, int>>{}]
           (int arg) mutable -> std::invoke_result_t<Fn, int> {
        auto [it, inserted] = cache.emplace(arg, std::invoke_result_t<Fn, int>{});
        if (inserted) it->second = std::invoke(fn, arg);
        return it->second;
    };
}

[[nodiscard]] auto curry(auto fn) {
    return [fn](auto a) {
        return [fn, a](auto b) {
            return std::invoke(fn, a, b);
        };
    };
}

template <std::invocable<int> Fn>
[[nodiscard]] auto negate_fn(Fn fn) {
    return [fn = std::move(fn)](int x) { return -std::invoke(fn, x); };
}

template <std::invocable<int> F, std::invocable<int> G>
[[nodiscard]] auto chain(F f, G g) {
    return [f = std::move(f), g = std::move(g)](int x) {
        return std::invoke(f, std::invoke(g, x));
    };
}

class Signal {
    std::vector<std::function<void(int)>> handlers_;
    int last_value_{};
public:
    void connect(std::function<void(int)> handler) {
        handlers_.push_back(std::move(handler));
    }
    void emit(int value) {
        last_value_ = value;
        std::ranges::for_each(handlers_, [&](const auto& h) { h(value); });
    }
    [[nodiscard]] int last_value() const noexcept { return last_value_; }
    [[nodiscard]] std::size_t connection_count() const noexcept { return handlers_.size(); }
};

int main() {
    std::vector<std::function<int()>> functions;
    functions.reserve(4);
    const int value = 10;
    functions.emplace_back([value]()                     { return value + 5;   });
    functions.emplace_back([value]()                     { return value * 2;   });
    functions.emplace_back([value, m = Multiplier{3}]()  { return m(value);    });
    functions.emplace_back([counter = 0]() mutable       { return ++counter;   });

    for (const auto& f : functions)
        std::cout << std::format("Result: {}\n", f());

    std::cout << "\n--- Direct std::function usage ---\n";
    std::function<int(int, int)> func = add;
    std::cout << std::format("add(3,4):      {}\n", func(3, 4));
    func = [](int a, int b) { return a * b; };
    std::cout << std::format("multiply(3,4): {}\n", func(3, 4));

    auto add_10 = std::bind_front(add, 10);
    std::cout << std::format("Bound result:  {}\n", add_10(5));

    std::cout << "\n--- Member function binding ---\n";
    Calculator calc;
    auto member_func = std::bind_front(&Calculator::increment, calc);
    std::cout << std::format("Member function result: {}\n", member_func(50));

    std::cout << "\n--- std::invoke ---\n";
    std::cout << std::format("Invoke add:     {}\n", std::invoke(add, 7, 8));
    std::cout << std::format("Invoke functor: {}\n", std::invoke(Multiplier{5}, 6));
    std::cout << std::format("Invoke member:  {}\n", std::invoke(&Calculator::increment, calc, 25));

    std::cout << "\n--- Callback dispatcher ---\n";
    std::vector<std::function<void()>> callbacks;
    callbacks.emplace_back([] { std::cout << "Callback A\n"; });
    callbacks.emplace_back([] { std::cout << "Callback B\n"; });
    std::ranges::for_each(callbacks, [](const auto& cb) { cb(); });

    std::cout << "\n--- Composed and reassigned functions ---\n";
    std::function<int(int)> square = [](int x) { return x * x; };
    std::cout << std::format("square(6): {}\n", square(6));

    const int base = 5;
    std::function<int(int)> add_base = [base](int x) { return x + base; };
    std::cout << std::format("add_base(10): {}\n", add_base(10));

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
    for (const auto& op : operations) std::cout << std::format("{} ", op(10));
    std::cout << '\n';

    std::cout << "\n--- Empty function check ---\n";
    std::function<void()> empty_func;
    if (!empty_func) std::cout << "Function is empty\n";

    std::cout << "\n--- Chained operations ---\n";
    std::function<int(int)> op_chain = [](int x) { return x + 2; };
    op_chain = [inner = std::move(op_chain)](int x) { return inner(x) * 3; };
    std::cout << std::format("Chained op(5): {}\n", op_chain(5));

    std::cout << "\n--- Mixed void(int) callable vector ---\n";
    const std::vector<std::function<void(int)>> printers = {
        [](int x) { std::cout << std::format("Val:    {}\n", x);     },
        [](int x) { std::cout << std::format("Square: {}\n", x * x); },
    };
    for (const auto& p : printers) p(4);

    std::cout << "\n--- Advanced Wrapper Utilities ---\n";
    std::cout << std::format("execute_operation(add, 7, 3): {}\n", execute_operation(add, 7, 3));

    EventDispatcher dispatcher;
    dispatcher.subscribe([] { std::cout << "Listener 1 triggered\n"; });
    dispatcher.subscribe([] { std::cout << "Listener 2 triggered\n"; });
    std::cout << std::format("Listener count: {}\n", dispatcher.listener_count());
    dispatcher.notify();

    const std::vector<std::function<int(int)>> pipeline = {
        [](int x) { return x + 5; },
        [](int x) { return x * 2; },
        [](int x) { return x - 1; },
    };
    std::cout << std::format("Pipeline result: {}\n", apply_pipeline(10, pipeline));

    std::cout << "\n--- Sorted descending ---\n";
    std::vector<int> nums = {5, 1, 4, 2, 3};
    std::ranges::sort(nums, std::greater<>{});
    for (int n : nums) std::cout << std::format("{} ", n);
    std::cout << '\n';

    std::cout << "\n--- Command Registry ---\n";
    CommandRegistry registry;
    registry.register_command("add", add);
    registry.register_command("sub", sub);
    registry.register_command("mul", [](int a, int b) { return a * b; });

    if (auto r = registry.run("add", 4, 5)) std::cout << std::format("add(4,5)={}\n", *r);
    if (auto r = registry.run("mul", 4, 5)) std::cout << std::format("mul(4,5)={}\n", *r);
    if (!registry.run("div", 4, 5))         std::cout << "div: not registered\n";
    std::cout << std::format("Registered commands: {}\n", registry.size());

    std::cout << "\n--- FunctionCache ---\n";
    int call_count = 0;
    FunctionCache<int(int)> cached_square([&](int x) { ++call_count; return x * x; });
    std::cout << std::format("cached_square(7)={} calls={}\n",     cached_square(7), call_count);
    std::cout << std::format("cached_square(7)={} calls={} (cached)\n", cached_square(7), call_count);
    std::cout << std::format("cached_square(9)={} calls={}\n",     cached_square(9), call_count);

    std::cout << "\n--- Overloaded visitor ---\n";
    auto visitor = Overloaded{
        [](int i)    { std::cout << std::format("int: {}\n",    i); },
        [](double d) { std::cout << std::format("double: {}\n", d); },
    };
    std::variant<int, double> v1 = 42, v2 = 3.14;
    std::visit(visitor, v1);
    std::visit(visitor, v2);

    std::cout << "\n--- compose / identity_fn ---\n";
    auto double_then_inc = compose([](int x) { return x + 1; }, [](int x) { return x * 2; });
    std::cout << std::format("double_then_inc(5)={}\n", double_then_inc(5));

    auto id = identity_fn<int>();
    std::cout << std::format("identity_fn(99)={}\n", id(99));

    std::cout << "\n--- memoize ---\n";
    int fib_calls = 0;
    auto slow_fib = [&](int n) -> int {
        ++fib_calls;
        if (n <= 1) return n;
        return n;
    };
    auto memo_fib = memoize(slow_fib);
    memo_fib(7); memo_fib(7); memo_fib(7);
    std::cout << std::format("memoize: fib(7) called {} times for 3 invocations\n", fib_calls);
    assert(fib_calls == 1);

    std::cout << "\n--- curry ---\n";
    auto curried_add = curry(add);
    auto add_five    = curried_add(5);
    std::cout << std::format("curry(add)(5)(3)={}\n", add_five(3));
    std::cout << std::format("curry(add)(5)(10)={}\n", add_five(10));
    assert(add_five(3) == 8);

    std::cout << "\n--- negate_fn / chain ---\n";
    auto neg_square = negate_fn([](int x) { return x * x; });
    std::cout << std::format("neg_square(4)={}\n", neg_square(4));

    auto add1_then_double = chain([](int x) { return x * 2; }, [](int x) { return x + 1; });
    std::cout << std::format("chain(double, add1)(3)={}\n", add1_then_double(3));
    assert(add1_then_double(3) == (3 + 1) * 2);

    std::cout << "\n--- Signal ---\n";
    Signal sig;
    std::vector<int> received;
    sig.connect([&](int v) { received.push_back(v);                    });
    sig.connect([](int v)  { std::cout << std::format("signal: {}\n", v); });
    sig.emit(42);
    sig.emit(99);
    std::cout << std::format("connection_count={} last_value={}\n",
                             sig.connection_count(), sig.last_value());
    assert(received == std::vector<int>{42, 99});
    assert(sig.last_value() == 99);

    assert(execute_operation(add, 2, 3) == 5);
    assert(registry.run("add", 1, 1).value() == 2);
    assert(!registry.run("nope", 1, 1).has_value());
    assert(cached_square(7) == 49);
    assert(call_count == 3);
    assert(double_then_inc(5) == 11);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
