// Object Slicing and Lifetime Exercise
// Understanding object slicing and lifetime rules

#include <iostream>
#include <memory>

class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() const { std::cout << "Animal\n"; }
};

class Dog : public Animal {
public:
    void speak() const override { std::cout << "Woof!\n"; }
};

int main() {
    // Object slicing problem (using value semantics)
    Dog dog;
    Animal animal = dog;  // Slicing!
    animal.speak();       // Calls Animal::speak, not Dog::speak
    
    // Solution: use pointers or references
    Animal& ref = dog;
    ref.speak();          // Calls Dog::speak correctly
    
    return 0;
}
