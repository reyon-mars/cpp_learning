// Stack vs Heap Exercise
// Memory allocation strategies and performance

#include <iostream>

class LargeObject {
public:
    int data[1000];
};

int main() {
    // Stack allocation (usually faster, automatic cleanup)
    LargeObject stack_obj;
    
    // Heap allocation (flexible, manual/smart pointer cleanup)
    auto heap_obj = new LargeObject();
    delete heap_obj;
    
    // Smart pointer (safe heap allocation)
    auto smart_ptr = std::make_unique<LargeObject>();
    // Automatic cleanup when out of scope
    
    return 0;
}
