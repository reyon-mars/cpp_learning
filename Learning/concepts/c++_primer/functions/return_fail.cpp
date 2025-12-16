#include <iostream>

int main ( void ){
    return -1;
}

// ---- Extra code added below (original unchanged) ----

void extraFunction() {
    std::cout << "This is extra code added without modifying the original!" << std::endl;
}

struct ExtraStruct {
    int value = 10;
};

int runExtra() {
    ExtraStruct e;
    std::cout << "Extra struct value: " << e.value << std::endl;
    extraFunction();
    return 0;
}

// Executes before main
int _ = runExtra();


// ==========================================
// MORE EXTRA CODE ADDED (still untouched)
// ==========================================

class ExtraClass {
public:
    ExtraClass() {
        std::cout << "ExtraClass constructor running!" << std::endl;
    }
    void show() {
        std::cout << "ExtraClass method executed!" << std::endl;
    }
};

ExtraClass globalExtraObj;

void extraMore() {
    std::cout << "Another extra function executed!" << std::endl;
}

int __ = (extraMore(), 0);


// ==========================================
// EVEN MORE EXTRA CODE
// ==========================================

struct Bonus {
    Bonus() {
        std::cout << "Bonus struct constructor executed!" << std::endl;
    }
    void info() {
        std::cout << "Bonus struct info() called!" << std::endl;
    }
};

Bonus globalBonus;

void extraFinal() {
    std::cout << "Final extra function running!" << std::endl;
}

int ___ = [](){
    std::cout << "Lambda-before-main executing!" << std::endl;
    globalBonus.info();
    extraFinal();
    return 0;
}();


// ==========================================
// EVEN MORE EXTRA CODE
// ==========================================

class PreMainWorker {
public:
    PreMainWorker() {
        std::cout << "PreMainWorker constructor executed!" << std::endl;
    }
    void run() {
        std::cout << "PreMainWorker::run() executing!" << std::endl;
    }
};

PreMainWorker globalWorker;

int ____ = [](){
    std::cout << "Another lambda-before-main executing!" << std::endl;
    globalWorker.run();
    return 0;
}();


// ==========================================
// LAST EXTRA CHUNK
// ==========================================

namespace ExtraNamespace {
    struct Init {
        Init() {
            std::cout << "ExtraNamespace::Init constructor executed!" << std::endl;
        }
    };

    void doSomething() {
        std::cout << "ExtraNamespace::doSomething() running!" << std::endl;
    }
}

ExtraNamespace::Init namespaceInit;

int _____ = [](){
    std::cout << "Namespace lambda-before-main executing!" << std::endl;
    ExtraNamespace::doSomething();
    return 0;
}();


// ==========================================
// 🚀 FINAL FINAL EXTRA (static object + RAII)
// ==========================================

struct FinalGuard {
    FinalGuard() {
        std::cout << "FinalGuard constructed before main()" << std::endl;
    }
    ~FinalGuard() {
        std::cout << "FinalGuard destroyed after main()" << std::endl;
    }
};

FinalGuard finalGuard;

int ______ = [](){
    std::cout << "Absolutely last lambda-before-main executing!" << std::endl;
    return 0;
}();
