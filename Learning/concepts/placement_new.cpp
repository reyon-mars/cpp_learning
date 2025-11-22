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

    return 0;
}