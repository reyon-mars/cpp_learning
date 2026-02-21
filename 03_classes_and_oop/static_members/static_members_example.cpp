// Static Members Exercise
// Class variables and member functions

#include <iostream>

class Counter {
private:
    int instance_count = 0;
    static int total_created;
    
public:
    Counter() { total_created++; }
    
    static int getTotalCreated() {
        return total_created;
    }
    
    static void printStats() {
        std::cout << "Total objects created: " << total_created << "\n";
    }
};

// Define and initialize static member
int Counter::total_created = 0;

int main() {
    Counter c1, c2, c3;
    
    std::cout << "Total created: " << Counter::getTotalCreated() << "\n";
    Counter::printStats();
    
    return 0;
}
