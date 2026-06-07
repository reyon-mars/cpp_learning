#include <iostream>
#include <typeinfo>
#include <utility>
#include <cassert>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <string_view>

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

    [[nodiscard]] T pop() {
        if (empty()) throw std::underflow_error{"Stack is empty"};
        return st_[top_--];
    }

    [[nodiscard]] const T& peek() const {
        if (empty()) throw std::underflow_error{"Stack is empty"};
        return st_[top_];
    }

    [[nodiscard]] bool  empty()    const noexcept { return top_ == -1;                      }
    [[nodiscard]] bool  full()     const noexcept { return top_ == static_cast<int>(Capacity) - 1; }
    [[nodiscard]] int   size()     const noexcept { return top_ + 1;                        }
    [[nodiscard]] std::size_t capacity() const noexcept { return Capacity;                  }

private:
    T   st_[Capacity]{};
    int top_ = -1;
};

template<typename T>
class Container {
public:
    Container() : data_{} {}

    explicit Container(const T& value) : data_{value}          {}
    explicit Container(T&& value)      : data_{std::move(value)} {}

    template<typename U>
    explicit Container(const Container<U>& other)
        : data_{static_cast<T>(other.get())} {}

    [[nodiscard]] T    get()        const          { return data_; }
    [[nodiscard]] bool is_default() const          { return data_ == T{}; }
    [[nodiscard]] bool equals(const T& other) const { return data_ == other; }

    [[nodiscard]] bool operator==(const Container<T>&) const = default;

    void set(const T& value)  { data_ = value;            }
    void set(T&& value)       { data_ = std::move(value); }
    void reset()              { data_ = T{};              }

    template<typename U>
    void convert_from(const U& other) { data_ = static_cast<T>(other); }

    void swap(Container<T>& other) noexcept {
        using std::swap;
        swap(data_, other.data_);
    }

    void print_type() const {
        std::cout << "Stored type: " << typeid(T).name() << "\n";
    }

    void print() const {
        std::cout << "Value: " << data_ << "\n";
    }

    friend std::ostream& operator<<(std::ostream& os, const Container<T>& c) {
        return os << c.data_;
    }

private:
    T data_;
};

template<typename T>
void print_container(const Container<T>& c) {
    std::cout << "[Container] " << c << "\n";
}

template<typename T>
[[nodiscard]] T max_value(const Container<T>& a, const Container<T>& b) {
    return a.get() > b.get() ? a.get() : b.get();
}

int main() {
    std::cout << "=== Stack ===\n";
    Stack<int> stack;
    stack.push(10);
    stack.push(20);
    stack.push(30);
    std::cout << "size=" << stack.size() << " peek=" << stack.peek() << "\n";
    std::cout << "pop=" << stack.pop() << "\n";
    std::cout << "pop=" << stack.pop() << "\n";
    std::cout << "empty=" << std::boolalpha << stack.empty() << "\n";

    Stack<std::string, 8> str_stack;
    str_stack.push("hello");
    str_stack.push("world");
    std::cout << "String stack peek=" << str_stack.peek() << "\n";

    std::cout << "\n=== Container<int> ===\n";
    Container<int> int_c{42};
    std::cout << "Int: " << int_c << "\n";
    int_c.print_type();

    std::cout << "\n=== Container<double> ===\n";
    Container<double> dbl_c{3.14};
    std::cout << "Double: " << dbl_c << "\n";
    dbl_c.convert_from(100);
    std::cout << "After convert_from(100): " << dbl_c << "\n";

    std::cout << "\n=== Cross-type construction ===\n";
    Container<int> from_dbl{dbl_c};
    std::cout << "int from double: " << from_dbl << "\n";

    std::cout << "\n=== Container<string> ===\n";
    Container<std::string> str_c;
    str_c.set("Templates!");
    std::cout << "String: " << str_c << "\n";
    std::cout << "is_default=" << str_c.is_default() << "\n";
    str_c.print_type();

    std::cout << "\n=== equals / swap ===\n";
    std::cout << "int_c.equals(42)=" << int_c.equals(42) << "\n";
    Container<int> other_int{10};
    std::cout << "Before swap: " << int_c << ", " << other_int << "\n";
    int_c.swap(other_int);
    std::cout << "After swap:  " << int_c << ", " << other_int << "\n";
    assert(int_c.equals(10));

    std::cout << "\n=== reset ===\n";
    other_int.reset();
    std::cout << "After reset: " << other_int << " is_default=" << other_int.is_default() << "\n";

    std::cout << "\n=== print helpers ===\n";
    print_container(int_c);
    print_container(dbl_c);

    std::cout << "\n=== max_value ===\n";
    std::cout << "Max=" << max_value(int_c, from_dbl) << "\n";

    std::cout << "\n=== operator== ===\n";
    Container<int> cmp{10};
    std::cout << "int_c == cmp: " << (int_c == cmp) << "\n";

    std::cout << "\n=== move set ===\n";
    Container<std::string> moved_str;
    std::string temp = "Move semantics";
    moved_str.set(std::move(temp));
    moved_str.print();
    std::cout << "Source after move: \"" << temp << "\"\n";

    std::cout << "\n=== vector<Container<int>> ===\n";
    std::vector<Container<int>> vec;
    vec.reserve(3);
    vec.emplace_back(1);
    vec.emplace_back(2);
    vec.emplace_back(3);
    for (const auto& c : vec) { std::cout << c << " "; }
    std::cout << "\n";

    std::cout << "\n=== sort descending ===\n";
    std::ranges::sort(vec, [](const auto& a, const auto& b) {
        return a.get() > b.get();
    });
    for (const auto& c : vec) { std::cout << c << " "; }
    std::cout << "\n";

    return 0;
}
