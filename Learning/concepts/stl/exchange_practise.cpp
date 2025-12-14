#include <iostream>
#include <utility>

class counter{
    private:
    int value;

    public:

    int increment( int amount ){
        return std::exchange( value, value + amount );
    }

    int reset ( ){
        return std::exchange( value, 0 );
    }
};


int main ( void ){

    counter countr;
    for( int i = 1; i < 100; i++ ){
        std::cout << countr.increment( i ) << " ";
    }
    std::cout << countr.reset(); 
    return 0;

}

// ----------------------------------------------------
// EXTRA CODE ADDED BELOW (original code untouched)
// ----------------------------------------------------

// Simple helper to test counter behavior
void test_counter(counter& c) {
    std::cout << "\n[Extra] Testing counter:\n";
    std::cout << "Increment by 5, previous = " << c.increment(5) << '\n';
    std::cout << "Increment by 10, previous = " << c.increment(10) << '\n';
    std::cout << "Reset, previous = " << c.reset() << '\n';
}

// Demonstrates repeated reset behavior
int runExtraCounterTests() {
    counter temp;
    test_counter(temp);
    std::cout << "[Extra] Reset again, previous = " << temp.reset() << '\n';
    return 0;
}

// Automatically executed before main()
int __ = runExtraCounterTests();
