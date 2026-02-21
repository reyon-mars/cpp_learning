// Functors and Callable Objects Exercise
// Function objects and custom callables

#include <iostream>
#include <vector>
#include <algorithm>

class Adder {
private:
    int value;
    
public:
    Adder(int v) : value(v) {}
    
    int operator()(int x) const { return x + value; }
};

class Counter {
private:
    mutable int count = 0;
    
public:
    void operator()() {
        ++count;
        std::cout << "Call count: " << count << "\n";
    }
};

int main() {
    std::vector<int> values = {1, 2, 3, 4, 5};
    
    // Transform with functor
    Adder add_10(10);
    std::transform(values.begin(), values.end(), values.begin(), add_10);
    
    for (int v : values) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    
    // Stateful functor
    Counter counter;
    counter();
    counter();
    counter();
    
    return 0;
}
