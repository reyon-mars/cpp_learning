#include <iostream>
#include <typeinfo>
#include <utility>
#include <cassert>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>
#include <concepts>
#include <numeric>
#include <functional>
#include <optional>
#include <span>

template<typename T, std::size_t Capacity = 64>
class Stack {
public:
    void push(const T& var) {
        if (full()) throw std::overflow_error{"Stack is full"};
        st_[++top_] = var;
    }

    void push(T&& var) {
        if (full()) throw std::overflow_error{"Stack is full"};
        st_[++top_] = std::move(var);
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        if (full()) throw std::overflow_error{"Stack is full"};
        st_[++top_] = T{std::forward<Args>(args)...};
    }

    [[nodiscard]] T pop() {
        if (empty()) throw std::underflow_error{"Stack is empty"};
        return st_[top_--];
    }

    [[nodiscard]] const T& peek() const {
        if (empty()) throw std::underflow_error{"Stack is empty"};
        return st_[top_];
    }

    void clear() noexcept { top_ = -1; }

    [[nodiscard]] bool        empty()    const noexcept { return top_ == -1; }
    [[nodiscard]] bool        full()     const noexcept { return top_ == static_cast<int>(Capacity) - 1; }
    [[nodiscard]] int         size()     const noexcept { return top_ + 1; }
    [[nodiscard]] std::size_t capacity() const noexcept { return Capacity; }

    void for_each(std::invocable<const T&> auto fn) const {
        for (int i = top_; i >= 0; --i) fn(st_[i]);
    }

    [[nodiscard]] bool contains(const T& value) const noexcept {
        for (int i = 0; i <= top_; ++i)
            if (st_[i] == value) return true;
        return false;
    }

    [[nodiscard]] std::vector<T> to_vector() const {
        return {st_.begin(), st_.begin() + top_ + 1};
    }

private:
    std::array<T, Capacity> st_{};
    int top_ = -1;
};

template<typename T>
concept Printable = requires(std::ostream& os, T t) { os << t; };

template<typename T>
concept Comparable = std::equality_comparable<T> && std::totally_ordered<T>;

template<typename T>
class Container {
public:
    Container() : data_{} {}

    explicit Container(const T& value) : data_{value}           {}
    explicit Container(T&& value)      : data_{std::move(value)} {}

    template<typename U>
    explicit Container(const Container<U>& other)
        : data_{static_cast<T>(other.get())} {}

    [[nodiscard]] T    get()               const { return data_; }
    [[nodiscard]] bool is_default()        const { return data_ == T{}; }
    [[nodiscard]] bool equals(const T& v)  const { return data_ == v; }

    [[nodiscard]] bool operator==(const Container<T>&) const = default;

    void set(const T& value) { data_ = value;            }
    void set(T&& value)      { data_ = std::move(value); }
    void reset()             { data_ = T{};              }

    template<typename U>
    void convert_from(const U& other) { data_ = static_cast<T>(other); }

    void swap(Container<T>& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
    }

    [[nodiscard]] Container<T> transformed(std::invocable<T> auto fn) const {
        return Container<T>{fn(data_)};
    }

    void apply(std::invocable<T&> auto fn) { fn(data_); }

    void print_type() const { std::cout << "type=" << typeid(T).name() << "\n"; }
    void print()      const { std::cout << "value=" << data_ << "\n"; }

    friend std::ostream& operator<<(std::ostream& os, const Container<T>& c) {
        return os << c.data_;
    }

private:
    T data_;
};

template<typename T>
void print_container(const Container<T>& c) { std::cout << "[Container] " << c << "\n"; }

template<typename T>
[[nodiscard]] T max_value(const Container<T>& a, const Container<T>& b) {
    return a.get() > b.get() ? a.get() : b.get();
}

template<typename T>
[[nodiscard]] T min_value(const Container<T>& a, const Container<T>& b) {
    return a.get() < b.get() ? a.get() : b.get();
}

template<Comparable T>
[[nodiscard]] Container<T> clamp(const Container<T>& c,
                                 const T& lo, const T& hi) {
    return Container<T>{std::clamp(c.get(), lo, hi)};
}

template<typename T>
[[nodiscard]] std::optional<T> find_in_stack(
        const Stack<T>& s, std::predicate<T> auto pred) {
    auto v = s.to_vector();
    auto it = std::ranges::find_if(v, pred);
    return it != v.end() ? std::optional<T>{*it} : std::nullopt;
}

int main() {
    std::cout << "=== Stack<int> ===\n";
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    std::cout << "size=" << stack.size() << " peek=" << stack.peek() << "\n"
              << "pop="  << stack.pop()  << "\n"
              << "pop="  << stack.pop()  << "\n"
              << "empty=" << std::boolalpha << stack.empty() << "\n";

    stack.push(5); stack.push(15); stack.push(25);
    std::cout << "for_each (top→bottom): ";
    stack.for_each([](const int& v){ std::cout << v << " "; });
    std::cout << "\n";

    std::cout << "contains(15)=" << stack.contains(15) << "\n";

    auto sv = stack.to_vector();
    std::cout << "to_vector: ";
    for (int v : sv) std::cout << v << " ";
    std::cout << "\n";

    if (auto found = find_in_stack(stack, [](int v){ return v > 10; }))
        std::cout << "first>10=" << *found << "\n";

    stack.clear();
    std::cout << "after clear empty=" << stack.empty() << "\n";

    std::cout << "\n=== Stack<string, 8> ===\n";
    Stack<std::string, 8> str_stack;
    str_stack.push("hello");
    str_stack.emplace("world");
    std::cout << "peek=" << str_stack.peek() << " size=" << str_stack.size() << "\n";

    std::cout << "\n=== Stack overflow guard ===\n";
    try {
        Stack<int, 2> tiny;
        tiny.push(1); tiny.push(2); tiny.push(3);
    } catch (const std::overflow_error& e) {
        std::cout << "overflow caught: " << e.what() << "\n";
    }

    std::cout << "\n=== Container<int> ===\n";
    Container<int> int_c{42};
    std::cout << int_c << "\n";
    int_c.print_type();

    auto doubled = int_c.transformed([](int v){ return v * 2; });
    std::cout << "transformed(*2)=" << doubled << "\n";

    int_c.apply([](int& v){ v += 8; });
    std::cout << "after apply(+8)=" << int_c << "\n";

    std::cout << "\n=== Container<double> ===\n";
    Container<double> dbl_c{3.14};
    dbl_c.convert_from(100);
    std::cout << "after convert_from(100)=" << dbl_c << "\n";

    std::cout << "\n=== Cross-type construction ===\n";
    Container<int> from_dbl{dbl_c};
    std::cout << "int from double=" << from_dbl << "\n";

    std::cout << "\n=== Container<string> ===\n";
    Container<std::string> str_c;
    str_c.set("Templates!");
    std::cout << str_c << " is_default=" << str_c.is_default() << "\n";
    str_c.print_type();

    std::cout << "\n=== equals / swap / reset ===\n";
    Container<int> other_int{10};
    std::cout << "int_c.equals(50)=" << int_c.equals(50) << "\n";
    std::cout << "before swap: " << int_c << ", " << other_int << "\n";
    int_c.swap(other_int);
    std::cout << "after swap:  " << int_c << ", " << other_int << "\n";
    assert(int_c.equals(10));
    other_int.reset();
    std::cout << "after reset: " << other_int << " is_default=" << other_int.is_default() << "\n";

    std::cout << "\n=== max / min / clamp ===\n";
    Container<int> a{5}, b{9};
    std::cout << "max=" << max_value(a, b) << " min=" << min_value(a, b) << "\n";
    Container<int> big{150};
    std::cout << "clamp(150,0,100)=" << clamp(big, 0, 100) << "\n";

    std::cout << "\n=== operator== ===\n";
    std::cout << "int_c==Container{10}: " << (int_c == Container<int>{10}) << "\n";

    std::cout << "\n=== move set ===\n";
    Container<std::string> moved_str;
    std::string temp = "Move semantics";
    moved_str.set(std::move(temp));
    moved_str.print();
    std::cout << "source after move: \"" << temp << "\"\n";

    std::cout << "\n=== vector<Container<int>> ===\n";
    std::vector<Container<int>> vec;
    vec.reserve(4);
    vec.emplace_back(3);
    vec.emplace_back(1);
    vec.emplace_back(4);
    vec.emplace_back(2);
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";

    std::ranges::sort(vec, [](const auto& x, const auto& y){ return x.get() < y.get(); });
    std::cout << "sorted: ";
    for (const auto& c : vec) std::cout << c << " ";
    std::cout << "\n";

    const int total = std::accumulate(vec.begin(), vec.end(), 0,
        [](int acc, const Container<int>& c){ return acc + c.get(); });
    std::cout << "sum=" << total << "\n";

    return 0;
}
