// Object Slicing and Lifetime Exercise
// Understanding object slicing and lifetime rules

#include <iostream>
#include <memory>
#include <vector>   // ✅ added

class Animal {
public:
    Animal() { std::cout << "Animal constructed\n"; }
    Animal(const Animal&) { std::cout << "Animal copied\n"; }
    virtual ~Animal() { std::cout << "Animal destroyed\n"; }

    virtual void speak() const { std::cout << "Animal\n"; }
};

class Dog : public Animal {
public:
    Dog() { std::cout << "Dog constructed\n"; }
    Dog(const Dog&) { std::cout << "Dog copied\n"; }
    ~Dog() { std::cout << "Dog destroyed\n"; }

    void speak() const override { std::cout << "Woof!\n"; }
};

// ----------- NEW ADDITIONS -----------

// Another derived class
class Cat : public Animal {
public:
    Cat() { std::cout << "Cat constructed\n"; }
    ~Cat() { std::cout << "Cat destroyed\n"; }

    void speak() const override { std::cout << "Meow!\n"; }
};

// Function using reference (no slicing)
void makeSpeak(const Animal& a) {
    a.speak();
}

// Function returning base pointer
Animal* createAnimal(bool isDog) {
    if (isDog)
        return new Dog();
    return new Cat();
}

// ------------------------------------

int main() {

    std::cout << "---- Object slicing ----\n";
    Dog dog;
    Animal animal = dog;  // Slicing occurs here
    animal.speak();       // Calls Animal::speak

    std::cout << "---- Passing by value (slicing again) ----\n";
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

    std::cout << "---- Dynamic allocation ----\n";
    Animal* dyn = new Dog();
    dyn->speak();
    delete dyn;

    std::cout << "---- Smart pointer (best practice) ----\n";
    std::unique_ptr<Animal> smartPtr = std::make_unique<Dog>();
    smartPtr->speak();

    // -------- NEW FEATURE USAGE --------

    std::cout << "---- Polymorphism with vector ----\n";
    std::vector<std::unique_ptr<Animal>> animals;
    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());

    for (const auto& a : animals) {
        a->speak();
    }

    std::cout << "---- Function with reference ----\n";
    makeSpeak(dog);

    std::cout << "---- Factory function ----\n";
    Animal* a1 = createAnimal(true);
    Animal* a2 = createAnimal(false);

    a1->speak();
    a2->speak();

    delete a1;
    delete a2;

    // ----------------------------------

    std::cout << "---- End of main ----\n";

    return 0;
}
