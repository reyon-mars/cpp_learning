// Functors and Callable Objects Exercise
// Function objects and custom callables

#include <iostream>
#include <vector>
#include <algorithm>

// ----------------------------------
// Simple Functor
// ----------------------------------
class Adder {
private:
    int value;

public:
    Adder(int v) : value(v) {}

    int operator()(int x) const { return x + value; }
};

// ----------------------------------
// Stateful Functor
// ----------------------------------
class Counter {
private:
    mutable int count = 0;

public:
    void operator()() const {
        ++count;
        std::cout << "Call count: " << count << "\n";
    }
};

// ----------------------------------
// Predicate Functor
// ----------------------------------
class GreaterThan {
private:
    int threshold;

public:
    GreaterThan(int t) : threshold(t) {}

    bool operator()(int x) const {
        return x > threshold;
    }
};

// ----------------------------------
// Generic Templated Functor
// ----------------------------------
template<typename T>
class Multiplier {
private:
    T factor;

public:
    Multiplier(T f) : factor(f) {}

    T operator()(T x) const {
        return x * factor;
    }
};

// ----------------------------------
// Main
// ----------------------------------
int main() {

    std::vector<int> values = {1, 2, 3, 4, 5};

    // Transform with functor
    Adder add_10(10);
    std::transform(values.begin(), values.end(),
                   values.begin(), add_10);

    for (int v : values)
        std::cout << v << " ";
    std::cout << "\n";

    // Stateful functor
    Counter counter;
    counter();
    counter();
    counter();

    std::cout << "\n--- Filtering with predicate ---\n";

    auto it = std::find_if(values.begin(),
                           values.end(),
                           GreaterThan(12));

    if (it != values.end())
        std::cout << "First value > 12: "
                  << *it << "\n";

    std::cout << "\n--- Sorting with functor ---\n";

    std::sort(values.begin(), values.end(),
              [](int a, int b) { return a > b; }); // lambda comparator

    for (int v : values)
        std::cout << v << " ";
    std::cout << "\n";

    std::cout << "\n--- Templated functor ---\n";

    Multiplier<int> mult(3);
    std::cout << "5 * 3 = "
              << mult(5) << "\n";

    std::cout << "\n--- Using functor in for_each ---\n";

    Counter algoCounter;
    std::for_each(values.begin(),
                  values.end(),
                  [&](int) { algoCounter(); });

    return 0;
}
