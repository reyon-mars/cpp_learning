// Object Slicing and Lifetime Exercise
// Understanding object slicing and lifetime rules

#include <iostream>
#include <memory>

class Animal {
public:
    Animal() { std::cout << "Animal constructed\n"; }
    Animal(const Animal&) { std::cout << "Animal copied\n"; }   // 🔹 added
    virtual ~Animal() { std::cout << "Animal destroyed\n"; }

    virtual void speak() const { std::cout << "Animal\n"; }
};

class Dog : public Animal {
public:
    Dog() { std::cout << "Dog constructed\n"; }
    Dog(const Dog&) { std::cout << "Dog copied\n"; }           // 🔹 added
    ~Dog() { std::cout << "Dog destroyed\n"; }

    void speak() const override { std::cout << "Woof!\n"; }
};

int main() {

    std::cout << "---- Object slicing ----\n";
    Dog dog;
    Animal animal = dog;  // Slicing occurs here
    animal.speak();       // Calls Animal::speak

    std::cout << "---- Passing by value (slicing again) ----\n";   // 🔹 added
    auto byValue = [](Animal a) {
        a.speak();
    };
    byValue(dog);

    std::cout << "---- Reference (no slicing) ----\n";
    Animal& ref = dog;
    ref.speak();

    std::cout << "---- Raw pointer (no slicing) ----\n";
    Animal* ptr = &dog;
    ptr->speak();

    std::cout << "---- Dynamic allocation ----\n";   // 🔹 added
    Animal* dyn = new Dog();
    dyn->speak();
    delete dyn;   // shows proper virtual destructor call

    std::cout << "---- Smart pointer (best practice) ----\n";
    std::unique_ptr<Animal> smartPtr = std::make_unique<Dog>();
    smartPtr->speak();

    std::cout << "---- End of main ----\n";

    return 0;
}
