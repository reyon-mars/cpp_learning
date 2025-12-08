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

// Even though main returns -1 and ends, this code still exists in the file.
int _ = runExtra();  // Executes extra code before main



// ==========================================
// MORE EXTRA CODE ADDED (still not modifying anything)
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

ExtraClass globalExtraObj;  // Runs before main()

void extraMore() {
    std::cout << "Another extra function executed!" << std::endl;
}

int __ = (extraMore(), 0); // Executes before main()
