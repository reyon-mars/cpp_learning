// ----------- MORE ADVANCED ADDITIONS -----------

#include <iostream>      // ✅ ADDED
#include <type_traits>   // ✅ ADDED

// ✅ ADDED: Packed struct (from earlier context)
#pragma pack(push, 1)
struct Packed {
    char c;
    int i;
    char d;
};
#pragma pack(pop)

// ✅ ADDED: Optimized struct
struct Optimized {
    int i;
    char c;
    char d;
};

// Struct with manual alignment
struct alignas(16) Aligned16 {
    int x;
    double y;
};

// Calculate padding manually
template <typename T>
void show_padding() {
    std::cout << "Total size: " << sizeof(T)
              << ", alignment: " << alignof(T) << "\n";
}

// ----------------------------------------------

int main() {

    std::cout << "\nAdvanced Layout Concepts:\n";

    // ✅ Padding insight
    std::cout << "Padding in Packed: "
              << sizeof(Packed) - (sizeof(char) + sizeof(int) + sizeof(char))
              << "\n";

    // ✅ Optimized struct check
    std::cout << "Optimized alignment: "
              << alignof(Optimized) << "\n";

    // ✅ Trivial copy check
    std::cout << "Packed is trivially copyable? "
              << std::is_trivially_copyable<Packed>::value << "\n";

    // ✅ Custom alignment
    Aligned16 a16{1, 2.5};
    std::cout << "Aligned16 size: " << sizeof(Aligned16)
              << ", alignment: " << alignof(Aligned16) << "\n";

    // ✅ Generic padding display
    show_padding<Optimized>();

    return 0;
}
