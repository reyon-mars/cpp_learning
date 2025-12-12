#include <iostream>

class base {
    private:
    int a;

    public:
    base(){ std::cout << "Constructor" << '\n'; }
    ~base(){ std::cout << "Destructor" << '\n'; }
};

int main( void ) {
    std::cout << "Normal case " << '\n';
    base* obj = new base();
    delete obj;

    std::cout << "Placement New Case" << '\n';
    char* memory = new char[ sizeof( base ) * 10 ];

    base* obj1 = new (&memory[0]) base();
    base* obj2 = new (&memory[sizeof(base)]) base();
    
    obj1->~base();
    obj2->~base();
    delete[] memory;

    return 0;
}



// ======================================================
// EXTRA CODE ADDED BELOW (original code untouched)
// ======================================================

class derived : public base {
public:
    derived() {
        std::cout << "Derived Constructor" << '\n';
    }
    ~derived() {
        std::cout << "Derived Destructor" << '\n';
    }
};

void test_normal_allocation() {
    std::cout << "\n[Extra] Normal Allocation for derived" << '\n';
    derived* d = new derived();
    delete d;
}

void test_placement_new() {
    std::cout << "\n[Extra] Placement-New Allocation for derived" << '\n';
    char* mem = new char[ sizeof(derived) * 2 ];

    derived* d1 = new (&mem[0]) derived();
    derived* d2 = new (&mem[ sizeof(derived) ]) derived();

    d1->~derived();
    d2->~derived();
    delete[] mem;
}

struct AutoRunTests {
    AutoRunTests() {
        std::cout << "\n=== Running Extra Tests Automatically ===\n";
        test_normal_allocation();
        test_placement_new();
    }
};

// Automatically runs before main()
AutoRunTests __auto_run_tests;
