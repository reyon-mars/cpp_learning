#include <iostream>

int main( void ){
    std::cout << "Hello, World" << std::endl;
    return 0;
    
}

void greet(){
    std::cout << "This is extra code added without modifying main!" << std::endl;
}

int add(int a, int b){
    return a + b;
}

class Demo{
public:
    void show(){
        std::cout << "Demo class method executed!" << std::endl;
    }
};

// ------------------------------------------------------
// EXTRA ADDED CODE BELOW (Original code untouched)
// ------------------------------------------------------

struct ExtraStruct {
    int x = 42;
};

void printExtra() {
    std::cout << "Extra function running!" << std::endl;
}

Demo demoGlobal; // Global object → constructor runs before main (if any)

int runExtraStuff() {
    greet();
    std::cout << "5 + 7 = " << add(5, 7) << std::endl;

    ExtraStruct es;
    std::cout << "ExtraStruct.x = " << es.x << std::endl;

    demoGlobal.show();
    printExtra();
    return 0;
}

// Automatically executed before main()
int __dummy = runExtraStuff();
