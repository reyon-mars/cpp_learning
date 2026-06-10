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

void throwing_function() {
    throw std::runtime_error("Error!");
}

void maybe_throws() noexcept(false) {
    throwing_function();
}

void safe_call() noexcept {
    try {
        maybe_throws();
    } catch (...) {
        std::cout << "Exception handled inside safe_call\n";
    }
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

template <std::invocable Func>
void test_noexcept(Func&& f) {
    std::cout << std::format("Is callable noexcept? {}\n", noexcept(f()));
}

template <typename T>
void conditional_noexcept(T value) noexcept(std::is_arithmetic_v<T>) {
    std::cout << std::format("Conditional noexcept executed with: {}\n", value);
}

template <std::invocable Func>
auto forward_call(Func&& f) noexcept(noexcept(std::invoke(std::forward<Func>(f)))) {
    return std::invoke(std::forward<Func>(f));
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

template <std::invocable Func>
[[nodiscard]] consteval bool is_noexcept_callable_type() {
    return noexcept(std::declval<Func>()());
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

int main() {
    std::vector<Widget> vec;
    vec.reserve(2);

    Widget w1(42);
    Widget w2(100);

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

    try {
        maybe_throws();
    } catch (const std::exception& e) {
        std::cout << std::format("Caught exception: {}\n", e.what());
    }

    std::cout << "\n--- Extra Tests ---\n";

    test_noexcept([]() noexcept { return 1; });
    test_noexcept([]() { return 2; });

    conditional_noexcept(10);
    conditional_noexcept(3.14);
    conditional_noexcept(std::string_view{"test"});

    auto result = forward_call([]() noexcept { return 123; });
    std::cout << std::format("Forward call result: {}\n", result);

    move_if_noexcept_demo();

    std::cout << "\n--- Advanced Noexcept Tests ---\n";

    Widget wa(1);
    Widget wb(2);

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
    for (int i : {10, 20, 30}) widgets.emplace_back(i);

    std::ranges::for_each(widgets, [](const Widget& w) {
        std::cout << std::format("Widget value: {}\n", w.get());
    });

    assert(noexcept(safe_lambda()));
    assert(noexcept(create_widget(5)));

    std::cout << "\nAssertions passed successfully\n";
    return 0;
}
