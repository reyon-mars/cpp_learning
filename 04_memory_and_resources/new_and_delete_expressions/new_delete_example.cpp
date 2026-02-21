// New and Delete Expressions Exercise
// Dynamic memory management with new/delete

#include <iostream>

class MyClass {
private:
    int value;
    
public:
    MyClass(int v) : value(v) { std::cout << "Constructor: " << v << "\n"; }
    ~MyClass() { std::cout << "Destructor\n"; }
};

int main() {
    // Single object
    MyClass* obj = new MyClass(42);
    delete obj;
    
    // Array
    int* arr = new int[10];
    for (int i = 0; i < 10; ++i) arr[i] = i;
    delete[] arr;
    
    return 0;
}
