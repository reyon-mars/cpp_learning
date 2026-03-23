// New and Delete Expressions Exercise
// Dynamic memory management with new/delete

#include <iostream>
#include <new>   // for std::size_t

class MyClass {
private:
    int value;

public:
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

    // ----------------------------------

    return 0;
}
