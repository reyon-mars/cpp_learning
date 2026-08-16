#include <iostream>
#include <vector>
#include <type_traits>
#include <stdexcept>
#include <utility>
#include <cassert>
#include <functional>
#include <concepts>
#include <format>
#include <span>
#include <algorithm>
#include <ranges>
#include <string_view>

class Widget {
    int data_;

public:
    explicit Widget(int d) noexcept : data_(d) {}

    Widget(const Widget&) = default;
    Widget& operator=(const Widget&) = default;

    Widget(Widget&& other) noexcept : data_(std::exchange(other.data_, 0)) {
        std::cout << "Move constructor (noexcept)\n";
    }

    Widget& operator=(Widget&& other) noexcept {
        std::cout << "Move assignment (noexcept)\n";
        if (this != &other)
            data_ = std::exchange(other.data_, 0);
        return *this;
    }

    ~Widget() = default;

    void process(int value) noexcept(std::is_integral_v<decltype(value)>) {
        data_ = value;
    }

    [[nodiscard]] int get() const noexcept { return data_; }

    friend void swap(Widget& a, Widget& b) noexcept {
        using std::swap;
        swap(a.data_, b.data_);
    }
};

static_assert(std::is_nothrow_move_constructible_v<Widget>);
static_assert(std::is_nothrow_move_assignable_v<Widget>);
static_assert(std::is_nothrow_swappable_v<Widget>);
static_assert(std::is_nothrow_destructible_v<Widget>);

class ThrowingMoveWidget {
    int data_;
public:
    explicit ThrowingMoveWidget(int d) noexcept : data_(d) {}
    ThrowingMoveWidget(ThrowingMoveWidget&& other) : data_(other.data_) {
        std::cout << "ThrowingMoveWidget: non-noexcept move (copy fallback expected)\n";
    }
    ThrowingMoveWidget(const ThrowingMoveWidget& other) : data_(other.data_) {
        std::cout << "ThrowingMoveWidget: copy constructor used\n";
    }
    ThrowingMoveWidget& operator=(ThrowingMoveWidget&&)      = default;
    ThrowingMoveWidget& operator=(const ThrowingMoveWidget&) = default;
    [[nodiscard]] int get() const noexcept { return data_; }
};

static_assert(!std::is_nothrow_move_constructible_v<ThrowingMoveWidget>);

template <std::invocable Func>
void test_noexcept(Func&& f) {
    std::cout << std::format("Is callable noexcept? {}\n", noexcept(f()));
}

template <typename T>
void check_noexcept() {
    std::cout << std::format("Is {} move construct noexcept? {}\n",
                             typeid(T).name(),
                             std::is_nothrow_move_constructible_v<T>);
}

[[nodiscard]] Widget create_widget(int val) noexcept {
    return Widget(val);
}

template <typename T>
void conditional_noexcept(T value) noexcept(std::is_arithmetic_v<T>) {
    std::cout << std::format("Conditional noexcept executed with: {}\n", value);
}

template <std::invocable Func>
auto forward_call(Func&& f) noexcept(noexcept(std::invoke(std::forward<Func>(f)))) {
    return std::invoke(std::forward<Func>(f));
}

void throwing_function() { throw std::runtime_error("Error!"); }
void maybe_throws() noexcept(false) { throwing_function(); }

void safe_call() noexcept {
    try {
        maybe_throws();
    } catch (...) {
        std::cout << "Exception handled inside safe_call\n";
    }
}

void move_if_noexcept_demo() {
    std::vector<Widget> v;
    v.reserve(2);
    Widget w(10);
    v.push_back(std::move_if_noexcept(w));
    std::cout << "move_if_noexcept used\n";
}

void safe_swap(Widget& a, Widget& b) noexcept {
    using std::swap;
    swap(a, b);
}

void execute_all(std::span<const std::function<void()>> funcs) noexcept {
    for (const auto& fn : funcs) {
        try {
            fn();
        } catch (const std::exception& e) {
            std::cout << std::format("Exception caught during execute_all: {}\n", e.what());
        } catch (...) {
            std::cout << "Unknown exception caught during execute_all\n";
        }
    }
}

constexpr auto safe_lambda = []() noexcept {
    std::cout << "Safe lambda executed\n";
};

template <std::invocable Func>
[[nodiscard]] auto make_noexcept_guard(Func&& f) noexcept {
    return [fn = std::forward<Func>(f)]() noexcept {
        try {
            std::invoke(fn);
        } catch (...) {
            std::cout << "Exception suppressed by noexcept guard\n";
        }
    };
}

template <typename Func>
class ScopeGuard {
    Func      fn_;
    bool      active_{true};
public:
    explicit ScopeGuard(Func fn) noexcept(std::is_nothrow_move_constructible_v<Func>)
        : fn_(std::move(fn)) {}

    ~ScopeGuard() noexcept {
        if (active_) {
            try { fn_(); }
            catch (...) { std::cout << "ScopeGuard: swallowed exception in destructor\n"; }
        }
    }

    void release() noexcept { active_ = false; }

    ScopeGuard(const ScopeGuard&)            = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
};

template <std::invocable Func>
[[nodiscard]] auto make_scope_guard(Func&& fn) noexcept(std::is_nothrow_move_constructible_v<Func>) {
    return ScopeGuard<std::remove_cvref_t<Func>>(std::forward<Func>(fn));
}

template <typename T>
struct noexcept_traits {
    static constexpr bool move_construct = std::is_nothrow_move_constructible_v<T>;
    static constexpr bool move_assign    = std::is_nothrow_move_assignable_v<T>;
    static constexpr bool destruct       = std::is_nothrow_destructible_v<T>;
    static constexpr bool copy_construct = std::is_nothrow_copy_constructible_v<T>;
    static constexpr bool swappable      = std::is_nothrow_swappable_v<T>;
    static constexpr bool all_noexcept   = move_construct && move_assign && destruct;

    static void print(std::string_view name) {
        std::cout << std::format("noexcept_traits<{}>:\n"
                                 "  move_construct={} move_assign={} destruct={}\n"
                                 "  copy_construct={} swappable={}  all={}\n",
                                 name,
                                 move_construct, move_assign, destruct,
                                 copy_construct, swappable,  all_noexcept);
    }
};

int main() {
    std::vector<Widget> vec;
    vec.reserve(2);

    Widget w1(42), w2(100);
    vec.push_back(std::move(w1));
    vec.push_back(std::move(w2));
    std::cout << std::format("Vector size: {}\n", vec.size());
    std::cout << std::format("Is Widget move constructor noexcept? {}\n",
                             noexcept(Widget(std::move(w1))));

    check_noexcept<Widget>();

    Widget w3 = create_widget(77);
    std::cout << std::format("Created widget value: {}\n", w3.get());

    safe_call();

    w3.process(999);
    std::cout << std::format("Processed value: {}\n", w3.get());

    try { maybe_throws(); }
    catch (const std::exception& e) { std::cout << std::format("Caught exception: {}\n", e.what()); }

    std::cout << "\n--- Extra Tests ---\n";

    test_noexcept([]() noexcept { return 1; });
    test_noexcept([]() { return 2; });

    conditional_noexcept(10);
    conditional_noexcept(3.14);
    conditional_noexcept(std::string_view{"test"});

    const auto result = forward_call([]() noexcept { return 123; });
    std::cout << std::format("Forward call result: {}\n", result);

    move_if_noexcept_demo();

    std::cout << "\n--- Advanced Noexcept Tests ---\n";

    Widget wa(1), wb(2);
    std::cout << std::format("Before swap: {}, {}\n", wa.get(), wb.get());
    safe_swap(wa, wb);
    std::cout << std::format("After swap: {}, {}\n", wa.get(), wb.get());

    static_assert(noexcept(safe_lambda()));
    static_assert(noexcept(create_widget(5)));
    static_assert(noexcept(safe_swap(wa, wb)));
    std::cout << "Compile-time noexcept checks passed\n";

    std::vector<std::function<void()>> tasks;
    tasks.push_back([]() noexcept { std::cout << "Task 1 completed\n"; });
    tasks.push_back([]() { throw std::runtime_error("Task failure"); });
    tasks.push_back([]() noexcept { std::cout << "Task 3 completed\n"; });
    execute_all(tasks);

    safe_lambda();

    std::cout << "\n--- noexcept guard ---\n";
    auto guarded = make_noexcept_guard([] { throw std::runtime_error("guarded throw"); });
    static_assert(noexcept(guarded()));
    guarded();

    std::cout << "\n--- noexcept propagation through std::ranges ---\n";
    std::vector<Widget> widgets;
    widgets.reserve(3);
    for (const int i : {10, 20, 30}) widgets.emplace_back(i);
    std::ranges::for_each(widgets, [](const Widget& w) {
        std::cout << std::format("Widget value: {}\n", w.get());
    });

    std::cout << "\n--- vector reallocation: noexcept move vs copy fallback ---\n";
    {
        std::cout << "Widget (noexcept move) reallocation:\n";
        std::vector<Widget> wv;
        wv.emplace_back(1);
        wv.emplace_back(2);
        wv.emplace_back(3);

        std::cout << "\nThrowingMoveWidget (non-noexcept move) reallocation:\n";
        std::vector<ThrowingMoveWidget> tv;
        tv.emplace_back(1);
        tv.emplace_back(2);
        tv.emplace_back(3);
    }

    std::cout << "\n--- ScopeGuard (noexcept destructor) ---\n";
    {
        auto guard = make_scope_guard([] {
            std::cout << "ScopeGuard cleanup executed\n";
        });
        std::cout << "Inside guarded scope\n";
    }

    {
        auto guard = make_scope_guard([] {
            std::cout << "Released guard cleanup (should NOT print)\n";
        });
        guard.release();
        std::cout << "Guard released — no cleanup\n";
    }

    {
        auto guard = make_scope_guard([] {
            throw std::runtime_error("thrown in scope guard");
        });
        std::cout << "Guard with throwing cleanup\n";
    }

    std::cout << "\n--- noexcept_traits<T> ---\n";
    noexcept_traits<Widget>::print("Widget");
    noexcept_traits<ThrowingMoveWidget>::print("ThrowingMoveWidget");
    noexcept_traits<std::string>::print("std::string");

    static_assert(noexcept_traits<Widget>::all_noexcept);
    static_assert(!noexcept_traits<ThrowingMoveWidget>::move_construct);

    assert(noexcept(safe_lambda()));
    assert(noexcept(create_widget(5)));
    std::cout << "\nAssertions passed successfully\n";
    return 0;
}
