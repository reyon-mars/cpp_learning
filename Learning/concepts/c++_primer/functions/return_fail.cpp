// ==========================================
// EXTRA CODE (APPEND ONLY – ORIGINAL UNCHANGED)
// ==========================================

#include <iostream>

// Extra function
void extraFunction() {
    std::cout << "This is extra code added without modifying the original!" << std::endl;
}

// Extra struct
struct ExtraStruct {
    int value = 10;
};

// Runs before main()
int runExtra() {
    ExtraStruct e;
    std::cout << "Extra struct value: " << e.value << std::endl;
    extraFunction();
    return 0;
}

int _ = runExtra();


// ------------------------------------------
// MORE EXTRA CODE
// ------------------------------------------

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


// ------------------------------------------
// EVEN MORE EXTRA CODE
// ------------------------------------------

struct Bonus {
    Bonus() {
        std::cout << "Bonus struct constructor executed!" << std::endl;
    }
    void info() {
        std::cout << "Bonus struct info() called!" << std::endl;
    }
};

Bonus globalBonus;

int __ = [](){
    std::cout << "Lambda-before-main executing!" << std::endl;
    globalBonus.info();
    return 0;
}();


// ------------------------------------------
// FINAL EXTRA CODE (RAII DEMO)
// ------------------------------------------

struct FinalGuard {
    FinalGuard() {
        std::cout << "FinalGuard constructed before main()" << std::endl;
    }
    ~FinalGuard() {
        std::cout << "FinalGuard destroyed after main()" << std::endl;
    }
};

FinalGuard finalGuard;
