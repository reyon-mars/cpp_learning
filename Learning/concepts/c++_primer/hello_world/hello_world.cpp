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

// ------------------------------------------------------
// MORE EXTRA CODE ADDED BELOW (still untouched above)
// ------------------------------------------------------

class MoreDemo {
public:
    void info() {
        std::cout << "MoreDemo class executing additional logic!" << std::endl;
    }
};

MoreDemo globalMoreDemo;

void printBanner() {
    std::cout << "=== Extra Banner ===" << std::endl;
}

int runMore() {
    printBanner();
    globalMoreDemo.info();
    std::cout << "Add(10, 20) = " << add(10, 20) << std::endl;
    return 0;
}

// Runs before main as well
int __dummy2 = runMore();

// ------------------------------------------------------
// EVEN MORE EXTRA CODE (still appended only)
// ------------------------------------------------------

struct Counter {
    Counter() {
        std::cout << "Counter constructed before main()" << std::endl;
    }
    int value = 100;
};

Counter globalCounter;

void showCounter() {
    std::cout << "Global counter value = " << globalCounter.value << std::endl;
}

int __dummy3 = [](){
    std::cout << "Lambda extra code executing!" << std::endl;
    showCounter();
    std::cout << "add(3, 4) = " << add(3, 4) << std::endl;
    return 0;
}();

// ------------------------------------------------------
// FINAL EXTRA BLOCK (no changes above)
// ------------------------------------------------------

class FinalDemo {
public:
    FinalDemo() {
        std::cout << "FinalDemo constructor running!" << std::endl;
    }
    void run() {
        std::cout << "FinalDemo::run() executed!" << std::endl;
    }
};

FinalDemo finalGlobal;

int __dummy4 = [](){
    finalGlobal.run();
    std::cout << "End of all extra code blocks." << std::endl;
    return 0;
}();
