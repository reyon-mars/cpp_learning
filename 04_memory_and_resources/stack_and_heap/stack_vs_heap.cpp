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

// ---------------- SMALL ADDITIONS ----------------

// Fill object with values
void fill_object(LargeObject& obj, int value) {
    for (int& x : obj.data) {
        x = value;
    }
}

// Calculate sum of elements
long long sum_object(const LargeObject& obj) {
    long long sum = 0;
    for (int x : obj.data) {
        sum += x;
    }
    return sum;
}

// Print first few elements (preview)
void print_preview(const LargeObject& obj, int count = 5) {
    std::cout << "Preview: ";
    for (int i = 0; i < count; ++i) {
        std::cout << obj.data[i] << " ";
    }
    std::cout << "\n";
}

// ------------------------------------------------

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

        // ---- added usage ----
        fill_object(stack_obj, 1);
        print_preview(stack_obj);
        std::cout << "Sum: " << sum_object(stack_obj) << "\n";
        // ---------------------

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

        // ---- added usage ----
        fill_object(*heap_obj, 2);
        print_preview(*heap_obj);
        std::cout << "Sum: " << sum_object(*heap_obj) << "\n";
        // ---------------------

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

        // ---- added usage ----
        fill_object(*smart_ptr, 3);
        print_preview(*smart_ptr);
        std::cout << "Sum: " << sum_object(*smart_ptr) << "\n";
        // ---------------------

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
