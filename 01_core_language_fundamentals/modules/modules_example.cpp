// Modules Exercise (C++20)
// Module interface and implementation units

// This would be in a module interface unit (.cppm)
// export module math_module;
// export int add(int a, int b) { return a + b; }

#include <iostream>

// ---- small added helper ----
void printModuleHint() {
    std::cout << "Modules replace headers in C++20\n";
}
// ----------------------------

int main() {

    std::cout << "C++20 Modules support\n";

    // ---- small added usage ----
    printModuleHint();
    // ---------------------------

    return 0;
}
