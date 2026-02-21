// STL Internals Exploration
// Understanding how STL containers are implemented

#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec;
    
    std::cout << "Vector capacity: " << vec.capacity() << "\n";
    std::cout << "Vector size: " << vec.size() << "\n";
    
    for (int i = 0; i < 10; ++i) {
        vec.push_back(i);
        std::cout << "After push_back(" << i << "): capacity=" << vec.capacity() 
                  << ", size=" << vec.size() << "\n";
    }
    
    // Reserve memory
    vec.reserve(100);
    std::cout << "After reserve(100): capacity=" << vec.capacity() << "\n";
    
    // Shrink to fit
    vec.shrink_to_fit();
    std::cout << "After shrink_to_fit: capacity=" << vec.capacity() << "\n";
    
    return 0;
}
