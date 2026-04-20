#include <iostream>
#include <memory>
#include <vector>

class Animal {
public:
    Animal() { std::cout << "Animal constructed\n"; }
    Animal(const Animal&) { std::cout << "Animal copied\n"; }
    
    // ✅ ADDED: move constructor
    Animal(Animal&&) noexcept { std::cout << "Animal moved\n"; }

    virtual ~Animal() { std::cout << "Animal destroyed\n"; }

    virtual void speak() const { std::cout << "Animal\n"; }
};

class Dog : public Animal {
public:
    Dog() { std::cout << "Dog constructed\n"; }
    Dog(const Dog&) { std::cout << "Dog copied\n"; }

    // ✅ ADDED: move constructor
    Dog(Dog&&) noexcept { std::cout << "Dog moved\n"; }

    ~Dog() override { std::cout << "Dog destroyed\n"; }

    void speak() const override { std::cout << "Woof!\n"; }
};

// ----------- NEW ADDITIONS -----------

class Cat : public Animal {
public:
    Cat() { std::cout << "Cat constructed\n"; }

    // ✅ ADDED: move constructor
    Cat(Cat&&) noexcept { std::cout << "Cat moved\n"; }

    ~Cat() override { std::cout << "Cat destroyed\n"; }

    void speak() const override { std::cout << "Meow!\n"; }
};

// ✅ ADDED: final class (cannot be inherited further)
class Bird final : public Animal {
public:
    Bird() { std::cout << "Bird constructed\n"; }
    ~Bird() override { std::cout << "Bird destroyed\n"; }

    void speak() const override { std::cout << "Chirp!\n"; }
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
    std::unique_ptr<Animal> smartPtr = std::make_unique<Dog>();
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

    // ----------------------------------

    std::cout << "---- End of main ----\n";

    return 0;
}
