#include <iostream>
#include <string>

// ======================================================
// ORIGINAL CODE (LOGIC UNCHANGED)
// ======================================================

void greet() {
    std::cout << "This is extra code added without modifying main!" << std::endl;
}

int add(int a, int b) {
    return a + b;
}

class Demo {
public:
    void show() {
        std::cout << "Demo class method executed!" << std::endl;
    }
};

// ======================================================
// EXTRA STRUCTS / CLASSES
// ======================================================

struct ExtraStruct {
    int x = 42;
};

class MoreDemo {
public:
    void info() {
        std::cout << "MoreDemo class executing additional logic!" << std::endl;
    }
};

struct Counter {
    int value = 100;
};

class FinalDemo {
public:
    void run() {
        std::cout << "FinalDemo::run() executed!" << std::endl;
    }
};

// ======================================================
// TEMPLATE + RAII
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

// ======================================================
// AUTO-RUN EXTRA DEMOS (BEFORE MAIN)
// ======================================================

int __pre_main_extra = [](){
    std::cout << "\n[Pre-main Extra Demo]\n";

    ExtraStruct es;
    std::cout << "Pre-main ExtraStruct.x = " << es.x << std::endl;

    MoreDemo more;
    more.info();

    Counter counter;
    std::cout << "Pre-main Counter value = " << counter.value << std::endl;

    FinalDemo finalDemo;
    finalDemo.run();

    std::cout << "multiply(3, 4) = " << multiply(3, 4) << std::endl;

    LifetimeTracer tracer("PreMainTracer");
    std::cout << "RAII object alive inside pre-main lambda\n";

    return 0;
}();

// ======================================================
// MAIN
// ======================================================

int main(void) {

    // ---------- ORIGINAL ----------
    std::cout << "Hello, World" << std::endl;

    // ---------- EXTRA FUNCTION CALLS ----------
    greet();
    std::cout << "5 + 7 = " << add(5, 7) << std::endl;

    // ---------- EXTRA STRUCT ----------
    ExtraStruct es;
    std::cout << "ExtraStruct.x = " << es.x << std::endl;

    // ---------- CLASS DEMOS ----------
    Demo demo;
    demo.show();

    MoreDemo more;
    more.info();

    // ---------- COUNTER ----------
    Counter counter;
    std::cout << "Counter value = " << counter.value << std::endl;

    // ---------- FINAL DEMO ----------
    FinalDemo finalDemo;
    finalDemo.run();

    // ---------- TEMPLATE ----------
    std::cout << "multiply(6, 7) = " << multiply(6, 7) << std::endl;

    // ---------- RAII ----------
    {
        LifetimeTracer tracer("MainScopeTracer");
        std::cout << "RAII object alive inside scope\n";
    }

    std::cout << "End of program execution." << std::endl;
    return 0;
}
