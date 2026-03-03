// Stack vs Heap Exercise
// Memory allocation strategies and performance

#include <iostream>
#include <memory>
#include <chrono>

class LargeObject {
public:
    int data[1000];

    LargeObject() {
        std::cout << "LargeObject constructed\n";
    }

    ~LargeObject() {
        std::cout << "LargeObject destroyed\n";
    }
};

int main() {

    // ---------------------------------------
    // Size comparison
    // ---------------------------------------
    std::cout << "Size of LargeObject: "
              << sizeof(LargeObject) << " bytes\n";

    // ---------------------------------------
    // Stack allocation (usually faster)
    // ---------------------------------------
    {
        auto start = std::chrono::steady_clock::now();

        LargeObject stack_obj;

        auto end = std::chrono::steady_clock::now();
        std::cout << "Stack allocation took: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
                  << " us\n";
    }

    std::cout << "---------------------\n";

    // ---------------------------------------
    // Heap allocation (manual cleanup)
    // ---------------------------------------
    {
        auto start = std::chrono::steady_clock::now();

        LargeObject* heap_obj = new LargeObject();

        auto end = std::chrono::steady_clock::now();
        std::cout << "Heap allocation took: "
                  << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
                  << " us\n";

        if (heap_obj != nullptr)
            std::cout << "Heap object allocated successfully\n";

        delete heap_obj;
    }

    std::cout << "---------------------\n";

    // ---------------------------------------
    // Smart pointer (safe heap allocation)
    // ---------------------------------------
    {
        auto smart_ptr = std::make_unique<LargeObject>();
        std::cout << "Smart pointer managing object\n";

        smart_ptr.reset();  // explicitly destroy early
        std::cout << "Smart pointer reset\n";
    }

    std::cout << "---------------------\n";

    // ---------------------------------------
    // Multiple allocation performance demo
    // ---------------------------------------
    {
        const int iterations = 10000;

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            LargeObject obj;
        }
        auto end = std::chrono::steady_clock::now();

        std::cout << "Stack loop time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms\n";
    }

    {
        const int iterations = 10000;

        auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            LargeObject* obj = new LargeObject();
            delete obj;
        }
        auto end = std::chrono::steady_clock::now();

        std::cout << "Heap loop time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                  << " ms\n";
    }

    return 0;
}
