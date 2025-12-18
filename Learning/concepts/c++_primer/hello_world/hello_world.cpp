#include <iostream>
#include <string>

// ======================================================
// ORIGINAL CODE (UNCHANGED)
// ======================================================

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

// ======================================================
// EXTRA ADDED CODE
// ======================================================

struct ExtraStruct {
    int x = 42;
};

void printExtra() {
    std::cout << "Extra function running!" << std::endl;
}

Demo demoGlobal;

int runExtraStuff() {
    greet();
    std::cout << "5 + 7 = " << add(5, 7) << std::endl;

    ExtraStruct es;
    std::cout << "ExtraStruct.x = " << es.x << std::endl;

    demoGlobal.show();
    printExtra();
    return 0;
}

int __dummy = runExtraStuff();

// ======================================================
// MORE EXTRA CODE
// ======================================================

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

int __dummy2 = runMore();

// ======================================================
// EVEN MORE EXTRA CODE
// ======================================================

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

// ======================================================
// FINAL EXTRA BLOCK
// ======================================================

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

// ======================================================
// 🔥 ULTRA EXTRA CODE (STATIC + TEMPLATE + RAII)
// ======================================================

template<typename T>
T multiply(T a, T b) {
    return a * b;
}

struct LifetimeTracer {
    std::string name;

    LifetimeTracer(std::string n) : name(std::move(n)) {
        std::cout << "[Lifetime] Construct " << name << std::endl;
    }

    ~LifetimeTracer() {
        std::cout << "[Lifetime] Destruct " << name << std::endl;
    }
};

static LifetimeTracer staticTracer("StaticTracer");

int __dummy5 = [](){
    LifetimeTracer local("LambdaTracer");
    std::cout << "multiply(6, 7) = " << multiply(6, 7) << std::endl;
    return 0;
}();

// ======================================================
// 🚀 END OF FILE
// ======================================================
