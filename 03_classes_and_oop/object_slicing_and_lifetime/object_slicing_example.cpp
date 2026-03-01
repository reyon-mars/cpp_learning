// Object Slicing and Lifetime Exercise
// Understanding object slicing and lifetime rules

#include <iostream>
#include <memory>

class Animal {
public:
    Animal() { std::cout << "Animal constructed\n"; }     // 🔹 added
    virtual ~Animal() { std::cout << "Animal destroyed\n"; }  // 🔹 expanded

    virtual void speak() const { std::cout << "Animal\n"; }
};

class Dog : public Animal {
public:
    Dog() { std::cout << "Dog constructed\n"; }           // 🔹 added
    ~Dog() { std::cout << "Dog destroyed\n"; }            // 🔹 added

    void speak() const override { std::cout << "Woof!\n"; }
};

int main() {

    std::cout << "---- Object slicing ----\n";
    Dog dog;
    Animal animal = dog;  // Slicing occurs here
    animal.speak();       // Calls Animal::speak

    std::cout << "---- Reference (no slicing) ----\n";
    Animal& ref = dog;
    ref.speak();          // Calls Dog::speak

    std::cout << "---- Raw pointer (no slicing) ----\n";
    Animal* ptr = &dog;
    ptr->speak();

    std::cout << "---- Smart pointer (best practice) ----\n";
    std::unique_ptr<Animal> smartPtr = std::make_unique<Dog>();
    smartPtr->speak();

    std::cout << "---- End of main ----\n";

    return 0;
}
