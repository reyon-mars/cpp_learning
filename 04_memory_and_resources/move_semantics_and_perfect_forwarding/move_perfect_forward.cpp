#include <iostream>
#include <utility>
#include <type_traits>
#include <vector>
#include <string_view>
#include <format>
#include <concepts>

class Widget {
public:
    int data;

    explicit Widget(int d) noexcept : data{d} {
        std::cout << std::format("Constructor({})\n", d);
    }

    Widget(const Widget& other) : data{other.data} {
        std::cout << "Copy constructor\n";
    }

    Widget(Widget&& other) noexcept : data{std::exchange(other.data, 0)} {
        std::cout << "Move constructor\n";
    }

    Widget& operator=(const Widget& other) {
        std::cout << "Copy assignment\n";
        data = other.data;
        return *this;
    }

    Widget& operator=(Widget&& other) noexcept {
        std::cout << "Move assignment\n";
        data = std::exchange(other.data, 0);
        return *this;
    }

    ~Widget() { std::cout << "Destructor\n"; }

    void show(std::string_view label) const {
        std::cout << std::format("{} data={}\n", label, data);
    }
};

void process(const Widget& w) { std::cout << std::format("process(const Widget&) data={}\n", w.data); }
void process(Widget&& w)      { std::cout << std::format("process(Widget&&)       data={}\n", w.data); }

void check(const Widget&) { std::cout << "check: const lvalue\n";     }
void check(Widget&)       { std::cout << "check: non-const lvalue\n"; }
void check(Widget&&)      { std::cout << "check: rvalue\n";           }

template <std::invocable<Widget&&> Func, typename... Args>
auto bad_forward_call(Func func, Args&&... args) {
    return func(args...);
}

template <typename Func, typename... Args>
    requires std::invocable<Func, Args...>
auto forward_call(Func func, Args&&... args) {
    return func(std::forward<Args>(args)...);
}

template <typename Func, typename... Args>
decltype(auto) forward_return(Func f, Args&&... args) {
    return f(std::forward<Args>(args)...);
}

template <typename T, typename... Args>
[[nodiscard]] T create_object(Args&&... args) {
    return T{std::forward<Args>(args)...};
}

template <typename T>
void detect_value_category(T&&) {
    std::cout << (std::is_lvalue_reference_v<T> ? "Lvalue\n" : "Rvalue\n");
}

template <typename T>
void wrapper(T&& arg) {
    std::cout << "wrapper -> ";
    process(std::forward<T>(arg));
}

template <typename T>
void compare_move_forward(T&& arg) {
    std::cout << "std::move:    ";
    process(std::move(arg));
    std::cout << "std::forward: ";
    process(std::forward<T>(arg));
}

template <typename T>
void reference_demo(T&&) {
    std::cout << std::format("is_lvalue_reference={}\n", std::is_lvalue_reference_v<T>);
}

template <typename T>
void double_forward(T&& arg) {
    std::cout << "1st forward: ";
    process(std::forward<T>(arg));
    std::cout << "2nd forward (may be moved-from): ";
    process(std::forward<T>(arg));
}

template <typename T>
void inspect(T&& value) {
    std::cout << (std::is_lvalue_reference_v<T> ? "inspect -> lvalue: " : "inspect -> rvalue: ");
    process(std::forward<T>(value));
}

[[nodiscard]] Widget generate_widget() {
    return Widget{1500};
}

void vector_forward_demo() {
    std::cout << "\n--- Vector forward demo ---\n";
    std::vector<Widget> widgets;
    widgets.reserve(3);
    for (const int val : {1, 2, 3}) widgets.emplace_back(val);
    std::cout << std::format("size={}\n", widgets.size());
}

int main() {
    static_assert(std::is_move_constructible_v<Widget>);
    static_assert(std::is_copy_constructible_v<Widget>);
    static_assert(std::is_nothrow_move_constructible_v<Widget>);

    constexpr auto call_process = [](auto&& w) { process(std::forward<decltype(w)>(w)); };

    std::cout << "\n--- Lvalue ---\n";
    Widget w{42};

    std::cout << "\n--- forward lvalue ---\n";
    forward_call(call_process, w);

    std::cout << "\n--- forward rvalue ---\n";
    forward_call(call_process, Widget{100});

    std::cout << "\n--- bad_forward rvalue (no forward) ---\n";
    bad_forward_call(call_process, Widget{200});

    std::cout << "\n--- forward std::move ---\n";
    forward_call(call_process, std::move(w));

    std::cout << "\n--- create_object (emplace-style) ---\n";
    Widget w2 = create_object<Widget>(300);

    std::cout << "\n--- Value category detection ---\n";
    detect_value_category(w2);
    detect_value_category(Widget{400});

    std::cout << "\n--- Overload resolution ---\n";
    check(w2);
    check(std::move(w2));
    const Widget cw{500};
    check(cw);

    std::cout << "\n--- wrapper ---\n";
    Widget w3{42};
    wrapper(w3);
    wrapper(Widget{600});

    std::cout << "\n--- compare move vs forward ---\n";
    Widget w4{800};
    compare_move_forward(w4);
    compare_move_forward(Widget{900});

    std::cout << "\n--- forward_return (decltype(auto)) ---\n";
    forward_return(call_process, Widget{1000});

    std::cout << "\n--- Reference collapsing ---\n";
    Widget w5{1100};
    reference_demo(w5);
    reference_demo(Widget{1101});

    std::cout << "\n--- Double forward pitfall ---\n";
    double_forward(Widget{1200});

    std::cout << "\n--- inspect (universal ref) ---\n";
    Widget w6{1300};
    inspect(w6);
    inspect(Widget{1400});

    vector_forward_demo();

    std::cout << "\n--- generate_widget (NRVO) ---\n";
    Widget generated = generate_widget();
    generated.show("generated");

    std::cout << "\n--- Type traits ---\n";
    std::cout << std::format(
        "move_constructible={}\ncopy_constructible={}\nnothrow_move_constructible={}\n",
        std::is_move_constructible_v<Widget>,
        std::is_copy_constructible_v<Widget>,
        std::is_nothrow_move_constructible_v<Widget>);

    return 0;
}
