// Concepts and Constraints Exercise (C++20)
// Compile-time constraints on template parameters

#include <iostream>
#include <concepts>

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

template<Numeric T>
T add(T a, T b) {
    return a + b;
}

template<typename T>
concept Container = requires(T t) {
    t.begin();
    t.end();
    t.size();
};

template<Container C>
void print_container(const C& cont) {
    std::cout << "Size: " << cont.size() << "\n";
}

int main() {
    std::cout << "Sum (int): " << add(5, 3) << "\n";
    std::cout << "Sum (double): " << add(1.5, 2.5) << "\n";
    
    std::vector<int> vec = {1, 2, 3};
    print_container(vec);
    
    return 0;
}
