// New and Delete Expressions Exercise
// Dynamic memory management with new/delete

#include <iostream>
#include <new>   // for std::size_t, std::nothrow

class MyClass {
private:
    int value;

public:
    // 🔹 Added default constructor (needed for array allocation)
    MyClass() : value(0) {
        std::cout << "Default Constructor\n";
    }

    MyClass(int v) : value(v) {
        std::cout << "Constructor: " << v << "\n";
    }

    ~MyClass() {
        std::cout << "Destructor: " << value << "\n";
    }

    // 🔹 Custom operator new
    static void* operator new(std::size_t size) {
        std::cout << "operator new called ("
                  << size << " bytes)\n";
        return ::operator new(size);
    }

    // 🔹 Custom operator delete
    static void operator delete(void* ptr) noexcept {
        std::cout << "operator delete called\n";
        ::operator delete(ptr);
    }

    // -------- NEW ADDITIONS --------

    // Array allocation
    static void* operator new[](std::size_t size) {
        std::cout << "operator new[] called ("
                  << size << " bytes)\n";
        return ::operator new(size);
    }

    static void operator delete[](void* ptr) noexcept {
        std::cout << "operator delete[] called\n";
        ::operator delete(ptr);
    }

    // 🔹 Sized delete (C++14+)
    static void operator delete(void* ptr, std::size_t size) noexcept {
        std::cout << "sized delete called (" << size << " bytes)\n";
        ::operator delete(ptr);
    }

    // 🔹 Placement delete (called if constructor throws)
    static void operator delete(void* ptr, void* place) noexcept {
        std::cout << "placement delete called\n";
    }

    // --------------------------------
};

int main() {

    std::cout << "\n--- Single Object ---\n";
    MyClass* obj = new MyClass(42);
    delete obj;

    std::cout << "\n--- Array of int ---\n";
    int* arr = new int[10];
    for (int i = 0; i < 10; ++i)
        arr[i] = i;
    delete[] arr;

    std::cout << "\n--- Array of MyClass ---\n";
    MyClass* objs = new MyClass[2]{ {1}, {2} };
    delete[] objs;

    // -------- NEW FEATURE USAGE --------

    std::cout << "\n--- Placement New ---\n";
    void* raw = ::operator new(sizeof(MyClass)); // raw memory
    MyClass* p = new(raw) MyClass(99);           // placement new

    p->~MyClass();  // manual destructor
    ::operator delete(raw);

    std::cout << "\n--- nothrow new ---\n";
    MyClass* safe = new(std::nothrow) MyClass(77);
    if (safe) {
        delete safe;
    } else {
        std::cout << "Allocation failed\n";
    }

    // 🔹 Manual array placement new
    std::cout << "\n--- Manual Array Placement ---\n";
    void* rawArr = ::operator new(sizeof(MyClass) * 2);
    MyClass* arrPtr = static_cast<MyClass*>(rawArr);

    new(&arrPtr[0]) MyClass(10);
    new(&arrPtr[1]) MyClass(20);

    arrPtr[0].~MyClass();
    arrPtr[1].~MyClass();
    ::operator delete(rawArr);

    // 🔹 Null pointer delete safety
    std::cout << "\n--- Deleting nullptr ---\n";
    MyClass* nullObj = nullptr;
    delete nullObj;  // safe

    // ----------------------------------

    return 0;
}
