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
#include <format>
#include <ranges>
#include <array>

template <typename T, std::size_t Capacity = 64>
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

    template <typename... Args>
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

    [[nodiscard]] std::span<const T> view() const noexcept {
        return {st_.data(), static_cast<std::size_t>(top_ + 1)};
    }

    void for_each(std::invocable<const T&> auto fn) const {
        for (int i = top_; i >= 0; --i) std::invoke(fn, st_[i]);
    }

    [[nodiscard]] bool contains(const T& value) const noexcept {
        return std::ranges::find(view(), value) != view().end();
    }

    [[nodiscard]] std::vector<T> to_vector() const {
        return {st_.begin(), st_.begin() + top_ + 1};
    }

    template <std::invocable<T> Fn>
    [[nodiscard]] std::vector<std::invoke_result_t<Fn, T>> map(Fn fn) const {
        std::vector<std::invoke_result_t<Fn, T>> result;
        result.reserve(static_cast<std::size_t>(top_ + 1));
        for (int i = 0; i <= top_; ++i)
            result.push_back(std::invoke(fn, st_[i]));
        return result;
    }

    template <typename U, std::invocable<U, T> BinOp>
    [[nodiscard]] U reduce(U init, BinOp op) const {
        for (int i = 0; i <= top_; ++i)
            init = std::invoke(op, std::move(init), st_[i]);
        return init;
    }

private:
    std::array<T, Capacity> st_{};
    int top_ = -1;
};

template <typename T>
class MinMaxStack {
    Stack<T>        data_;
    Stack<T>        min_st_;
    Stack<T>        max_st_;

public:
    void push(T val) {
        data_.push(val);
        min_st_.push(min_st_.empty() ? val : std::min(val, min_st_.peek()));
        max_st_.push(max_st_.empty() ? val : std::max(val, max_st_.peek()));
    }

    [[nodiscard]] T pop() {
        min_st_.pop();
        max_st_.pop();
        return data_.pop();
    }

    [[nodiscard]] const T& peek()    const { return data_.peek(); }
    [[nodiscard]] const T& current_min() const { return min_st_.peek(); }
    [[nodiscard]] const T& current_max() const { return max_st_.peek(); }
    [[nodiscard]] bool     empty()   const noexcept { return data_.empty(); }
    [[nodiscard]] int      size()    const noexcept { return data_.size(); }
};

template <typename T>
concept Printable = requires(std::ostream& os, T t) { os << t; };

template <typename T>
concept Comparable = std::equality_comparable<T> && std::totally_ordered<T>;

template <typename T>
class Container {
public:
    Container() : data_{} {}
    explicit Container(const T& value) : data_{value}            {}
    explicit Container(T&& value)      : data_{std::move(value)} {}

    template <typename U>
    explicit Container(const Container<U>& other)
        : data_{static_cast<T>(other.get())} {}

    [[nodiscard]] T    get()             const { return data_; }
    [[nodiscard]] bool is_default()      const { return data_ == T{}; }
    [[nodiscard]] bool equals(const T& v) const { return data_ == v; }

    [[nodiscard]] bool operator==(const Container<T>&) const = default;

    void set(const T& value) { data_ = value;            }
    void set(T&& value)      { data_ = std::move(value); }
    void reset()             { data_ = T{};              }

    template <typename U>
    void convert_from(const U& other) { data_ = static_cast<T>(other); }

    void swap(Container<T>& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
    }

    [[nodiscard]] Container<T> transformed(std::invocable<T> auto fn) const {
        return Container<T>{std::invoke(fn, data_)};
    }

    void apply(std::invocable<T&> auto fn) { std::invoke(fn, data_); }

    void print_type() const { std::cout << std::format("type={}\n", typeid(T).name()); }
    void print()      const { std::cout << std::format("value={}\n", data_); }

    friend std::ostream& operator<<(std::ostream& os, const Container<T>& c) {
        return os << c.data_;
    }

private:
    T data_;
};

template <typename T>
void print_container(const Container<T>& c) {
    std::cout << std::format("[Container] {}\n", c.get());
}

template <typename T>
[[nodiscard]] T max_value(const Container<T>& a, const Container<T>& b) {
    return a.get() > b.get() ? a.get() : b.get();
}

template <typename T>
[[nodiscard]] T min_value(const Container<T>& a, const Container<T>& b) {
    return a.get() < b.get() ? a.get() : b.get();
}

template <Comparable T>
[[nodiscard]] Container<T> clamp(const Container<T>& c, const T& lo, const T& hi) {
    return Container<T>{std::clamp(c.get(), lo, hi)};
}

template <typename T>
[[nodiscard]] std::optional<T> find_in_stack(
        const Stack<T>& s, std::predicate<T> auto pred) {
    auto v  = s.to_vector();
    auto it = std::ranges::find_if(v, pred);
    return it != v.end() ? std::optional<T>{*it} : std::nullopt;
}

int main() {
    std::cout << "=== Stack<int> ===\n";
    Stack<int> stack;
    stack.push(10); stack.push(20); stack.push(30);
    std::cout << std::format("size={} peek={}\n", stack.size(), stack.peek());
    std::cout << std::format("pop={}\npop={}\n", stack.pop(), stack.pop());
    std::cout << std::format("empty={}\n", stack.empty());

    stack.push(5); stack.push(15); stack.push(25);
    std::cout << "for_each (top→bottom): ";
    stack.for_each([](const int& v) { std::cout << std::format("{} ", v); });
    std::cout << '\n';

    std::cout << std::format("contains(15)={}\n", stack.contains(15));

    std::cout << "to_vector: ";
    for (int v : stack.to_vector()) std::cout << std::format("{} ", v);
    std::cout << '\n';

    std::cout << "map(*2): ";
    for (int v : stack.map([](int x) { return x * 2; }))
        std::cout << std::format("{} ", v);
    std::cout << '\n';

    const int stack_sum = stack.reduce(0, std::plus<>{});
    std::cout << std::format("reduce(sum)={}\n", stack_sum);

    std::cout << "view (span): ";
    for (int v : stack.view()) std::cout << std::format("{} ", v);
    std::cout << '\n';

    if (auto found = find_in_stack(stack, [](int v) { return v > 10; }))
        std::cout << std::format("first>10={}\n", *found);

    stack.clear();
    std::cout << std::format("after clear empty={}\n", stack.empty());

    std::cout << "\n=== Stack<string, 8> ===\n";
    Stack<std::string, 8> str_stack;
    str_stack.push("hello");
    str_stack.emplace("world");
    std::cout << std::format("peek={} size={}\n", str_stack.peek(), str_stack.size());

    std::cout << "\n=== Stack overflow guard ===\n";
    try {
        Stack<int, 2> tiny;
        tiny.push(1); tiny.push(2); tiny.push(3);
    } catch (const std::overflow_error& e) {
        std::cout << std::format("overflow caught: {}\n", e.what());
    }

    std::cout << "\n=== MinMaxStack ===\n";
    MinMaxStack<int> mm;
    for (int v : {3, 1, 7, 4, 2}) {
        mm.push(v);
        std::cout << std::format("push({}) -> min={} max={}\n", v, mm.current_min(), mm.current_max());
    }
    mm.pop();
    std::cout << std::format("after pop: min={} max={}\n", mm.current_min(), mm.current_max());

    std::cout << "\n=== Container<int> ===\n";
    Container<int> int_c{42};
    std::cout << std::format("{}\n", int_c.get());
    int_c.print_type();

    auto doubled = int_c.transformed([](int v) { return v * 2; });
    std::cout << std::format("transformed(*2)={}\n", doubled.get());

    int_c.apply([](int& v) { v += 8; });
    std::cout << std::format("after apply(+8)={}\n", int_c.get());

    std::cout << "\n=== Container<double> ===\n";
    Container<double> dbl_c{3.14};
    dbl_c.convert_from(100);
    std::cout << std::format("after convert_from(100)={}\n", dbl_c.get());

    std::cout << "\n=== Cross-type construction ===\n";
    Container<int> from_dbl{dbl_c};
    std::cout << std::format("int from double={}\n", from_dbl.get());

    std::cout << "\n=== Container<string> ===\n";
    Container<std::string> str_c;
    str_c.set("Templates!");
    std::cout << std::format("{} is_default={}\n", str_c.get(), str_c.is_default());
    str_c.print_type();

    std::cout << "\n=== equals / swap / reset ===\n";
    Container<int> other_int{10};
    std::cout << std::format("int_c.equals(50)={}\n", int_c.equals(50));
    std::cout << std::format("before swap: {}, {}\n", int_c.get(), other_int.get());
    int_c.swap(other_int);
    std::cout << std::format("after swap:  {}, {}\n", int_c.get(), other_int.get());
    assert(int_c.equals(10));
    other_int.reset();
    std::cout << std::format("after reset: {} is_default={}\n",
                             other_int.get(), other_int.is_default());

    std::cout << "\n=== max / min / clamp ===\n";
    Container<int> a{5}, b{9};
    std::cout << std::format("max={} min={}\n", max_value(a, b), min_value(a, b));
    Container<int> big{150};
    std::cout << std::format("clamp(150,0,100)={}\n", clamp(big, 0, 100).get());

    std::cout << "\n=== operator== ===\n";
    std::cout << std::format("int_c==Container{{10}}: {}\n", int_c == Container<int>{10});

    std::cout << "\n=== move set ===\n";
    Container<std::string> moved_str;
    std::string temp = "Move semantics";
    moved_str.set(std::move(temp));
    moved_str.print();
    std::cout << std::format("source after move: \"{}\"\n", temp);

    std::cout << "\n=== vector<Container<int>> ===\n";
    std::vector<Container<int>> vec;
    vec.reserve(4);
    vec.emplace_back(3); vec.emplace_back(1);
    vec.emplace_back(4); vec.emplace_back(2);
    for (const auto& c : vec) std::cout << std::format("{} ", c.get());
    std::cout << '\n';

    std::ranges::sort(vec, {}, &Container<int>::get);
    std::cout << "sorted: ";
    for (const auto& c : vec) std::cout << std::format("{} ", c.get());
    std::cout << '\n';

    const int total = std::reduce(vec.begin(), vec.end(), 0,
        [](int acc, const Container<int>& c) { return acc + c.get(); });
    std::cout << std::format("sum={}\n", total);

    std::cout << "\n--- views pipeline over Container vector ---\n";
    auto doubled_view = vec
        | std::views::transform([](const Container<int>& c) { return c.get() * 2; });
    std::cout << "doubled: ";
    for (int v : doubled_view) std::cout << std::format("{} ", v);
    std::cout << '\n';

    assert(total == 10);
    assert(clamp(big, 0, 100).get() == 100);
    assert(max_value(a, b) == 9);
    assert(min_value(a, b) == 5);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
