#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <utility>   // ✅ ADDED
#include <cassert>   // ✅ ADDED

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

// -------- NEW ADDITIONS --------

// Compare two objects
bool compare_objects(const LargeObject& a, const LargeObject& b) {
    for (int i = 0; i < 1000; ++i) {
        if (a.data[i] != b.data[i])
            return false;
    }
    return true;
}

// Copy data from one object to another
void copy_object(LargeObject& dest, const LargeObject& src) {
    for (int i = 0; i < 1000; ++i) {
        dest.data[i] = src.data[i];
    }
}

// Measure average allocation time
void measure_average_stack(int iterations) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        LargeObject obj;
    }
    auto end = std::chrono::steady_clock::now();

    std::cout << "Avg stack time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / iterations
              << " us\n";
}

void measure_average_heap(int iterations) {
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < iterations; ++i) {
        LargeObject* obj = new LargeObject();
        delete obj;
    }
    auto end = std::chrono::steady_clock::now();

    std::cout << "Avg heap time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / iterations
              << " us\n";
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
        assert(&stack_obj != nullptr);  // ✅ ADDED safety check

        fill_object(stack_obj, 1);
        print_preview(stack_obj);
        std::cout << "Sum: " << sum_object(stack_obj) << "\n";

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
        assert(heap_obj != nullptr);  // ✅ ADDED

        fill_object(*heap_obj, 2);
        print_preview(*heap_obj);
        std::cout << "Sum: " << sum_object(*heap_obj) << "\n";

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
        assert(smart_ptr);  // ✅ ADDED

        std::cout << "Smart pointer managing object\n";

        fill_object(*smart_ptr, 3);
        print_preview(*smart_ptr);
        std::cout << "Sum: " << sum_object(*smart_ptr) << "\n";

        smart_ptr.reset();
        std::cout << "Smart pointer reset\n";
    }

    std::cout << "---------------------\n";

    // -------- NEW FEATURE USAGE --------

    {
        std::cout << "\n--- Object Comparison & Copy ---\n";
        LargeObject a, b;

        fill_object(a, 5);
        copy_object(b, a);

        std::cout << "Objects equal? "
                  << (compare_objects(a, b) ? "Yes\n" : "No\n");
    }

    {
        std::cout << "\n--- Shared Pointer Demo ---\n";
        std::shared_ptr<LargeObject> sp1 = std::make_shared<LargeObject>();
        std::shared_ptr<LargeObject> sp2 = sp1;

        std::cout << "Use count: " << sp1.use_count() << "\n";
    }

    {
        std::cout << "\n--- Vector of Heap Objects ---\n";
        std::vector<std::unique_ptr<LargeObject>> vec;

        for (int i = 0; i < 3; ++i) {
            vec.push_back(std::make_unique<LargeObject>());
            fill_object(*vec.back(), i);
        }

        for (const auto& obj : vec) {
            std::cout << "Sum: " << sum_object(*obj) << "\n";
        }
    }

    {
        std::cout << "\n--- Lambda Processing ---\n";
        LargeObject obj;
        fill_object(obj, 7);

        auto compute = [](const LargeObject& o) {
            return sum_object(o);
        };

        std::cout << "Lambda sum: " << compute(obj) << "\n";
    }

    {
        std::cout << "\n--- Average Timing ---\n";
        measure_average_stack(1000);
        measure_average_heap(1000);
    }

    // ---------------------------------------

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
