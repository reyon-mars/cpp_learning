#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>   // ✅ ADDED
#include <cassert>    // ✅ ADDED
#include <utility>    // ✅ ADDED

class Animal {
public:
    Animal() {
        std::cout << "Animal constructed\n";
    }

    Animal(const Animal&) {
        std::cout << "Animal copied\n";
    }
    
    // ✅ ADDED: move constructor
    Animal(Animal&&) noexcept {
        std::cout << "Animal moved\n";
    }

    virtual ~Animal() {
        std::cout << "Animal destroyed\n";
    }

    virtual void speak() const {
        std::cout << "Animal\n";
    }

    // -------- EXTRA ADDITION --------
    virtual void info() const {
        std::cout << "Generic Animal\n";
    }
    // --------------------------------
};

class Dog : public Animal {
public:
    Dog() {
        std::cout << "Dog constructed\n";
    }

    Dog(const Dog&) {
        std::cout << "Dog copied\n";
    }

    // ✅ ADDED: move constructor
    Dog(Dog&&) noexcept {
        std::cout << "Dog moved\n";
    }

    ~Dog() override {
        std::cout << "Dog destroyed\n";
    }

    void speak() const override {
        std::cout << "Woof!\n";
    }

    // -------- EXTRA ADDITION --------
    void info() const override {
        std::cout << "This is a Dog\n";
    }
    // --------------------------------
};

// ----------- NEW ADDITIONS -----------

class Cat : public Animal {
public:
    Cat() {
        std::cout << "Cat constructed\n";
    }

    // ✅ ADDED: move constructor
    Cat(Cat&&) noexcept {
        std::cout << "Cat moved\n";
    }

    ~Cat() override {
        std::cout << "Cat destroyed\n";
    }

    void speak() const override {
        std::cout << "Meow!\n";
    }

    // -------- EXTRA ADDITION --------
    void info() const override {
        std::cout << "This is a Cat\n";
    }
    // --------------------------------
};

// ✅ ADDED: final class (cannot be inherited further)
class Bird final : public Animal {
public:
    Bird() {
        std::cout << "Bird constructed\n";
    }

    ~Bird() override {
        std::cout << "Bird destroyed\n";
    }

    void speak() const override {
        std::cout << "Chirp!\n";
    }

    // -------- EXTRA ADDITION --------
    void info() const override {
        std::cout << "This is a Bird\n";
    }
    // --------------------------------
};

// Function using reference (no slicing)
void makeSpeak(const Animal& a) {
    a.speak();
}

// Raw pointer factory
Animal* createAnimal(bool isDog) {
    if (isDog)
        return new Dog();

    return new Cat();
}

// ✅ ADDED: smart factory (recommended)
std::unique_ptr<Animal> createAnimalSmart(bool isDog) {
    if (isDog)
        return std::make_unique<Dog>();

    return std::make_unique<Cat>();
}

// ✅ ADDED: lifetime tracker
void lifetimeTracker() {
    std::cout << "\n[Lifetime Tracker]\n";

    Animal a;
}

// ----------- MORE ADVANCED ADDITIONS -----------

// RTTI demo
void printType(const Animal& a) {
    std::cout << "RTTI type: "
              << typeid(a).name() << "\n";
}

// dynamic_cast demo
void checkDog(const Animal* a) {
    if (const Dog* d = dynamic_cast<const Dog*>(a)) {
        std::cout << "dynamic_cast success → Dog detected\n";
        d->speak();
    }
    else {
        std::cout << "dynamic_cast failed → Not a Dog\n";
    }
}

// Pass by rvalue reference
void consumeAnimal(Animal&& a) {
    std::cout << "Consuming temporary animal\n";
    a.speak();
}

// Move semantics demo
void moveDemo() {
    std::cout << "\n[Move Demo]\n";

    Dog d1;
    Dog d2 = std::move(d1);

    d2.speak();
}

// Smart pointer ownership transfer
void ownershipTransfer() {
    std::cout << "\n[Ownership Transfer]\n";

    std::unique_ptr<Animal> ptr1 = std::make_unique<Dog>();

    assert(ptr1 != nullptr);

    std::unique_ptr<Animal> ptr2 = std::move(ptr1);

    if (!ptr1) {
        std::cout << "ptr1 is now null after move\n";
    }

    ptr2->speak();
}

// ------------------------------------

int main() {

    std::cout << "---- Object slicing ----\n";

    Dog dog;

    Animal animal = dog;  // Slicing occurs here
    animal.speak();

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

    std::unique_ptr<Animal> smartPtr =
        std::make_unique<Dog>();

    smartPtr->speak();

    // -------- NEW FEATURE USAGE --------

    std::cout << "---- Polymorphism with vector ----\n";

    std::vector<std::unique_ptr<Animal>> animals;

    // ✅ emplace_back (more efficient)
    animals.emplace_back(std::make_unique<Dog>());
    animals.emplace_back(std::make_unique<Cat>());
    animals.emplace_back(std::make_unique<Bird>());

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

    // ✅ ADDED: smart factory usage
    std::cout << "---- Smart Factory ----\n";

    auto s1 = createAnimalSmart(true);
    auto s2 = createAnimalSmart(false);

    s1->speak();
    s2->speak();

    // ✅ ADDED: lifetime tracking
    lifetimeTracker();

    // -------- EXTRA ADVANCED USAGE --------

    std::cout << "---- RTTI Demo ----\n";

    printType(dog);

    Cat cat;
    printType(cat);

    std::cout << "---- dynamic_cast Demo ----\n";

    checkDog(&dog);
    checkDog(&cat);

    std::cout << "---- Virtual info() Demo ----\n";

    dog.info();
    cat.info();

    Bird bird;
    bird.info();

    std::cout << "---- Rvalue Reference Demo ----\n";

    consumeAnimal(Dog());

    std::cout << "---- Move Constructor Demo ----\n";

    moveDemo();

    std::cout << "---- Ownership Transfer Demo ----\n";

    ownershipTransfer();

    std::cout << "---- Vector Iteration with info() ----\n";

    for (const auto& a : animals) {
        a->info();
    }

    // ----------------------------------

    std::cout << "---- End of main ----\n";

    return 0;
}
