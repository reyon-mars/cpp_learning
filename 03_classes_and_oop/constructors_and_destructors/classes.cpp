#include <algorithm>
#include <cassert>
#include <concepts>
#include <cstdlib>
#include <cxxabi.h>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <typeinfo>
#include <vector>

class Animal {
protected:
    std::string name_;

public:
    explicit Animal(std::string_view name) : name_{name} {}
    virtual ~Animal() = default;

    virtual void makeSound() const {
        std::cout << name_ << " is making a sound\n";
    }

    void wakeUp() const {
        std::cout << name_ << " just woke up — ";
        makeSound();
    }

    [[nodiscard]] std::string_view getName() const noexcept { return name_; }
};

class Pet {
protected:
    std::string name_;

public:
    explicit Pet(std::string_view name) : name_{name} {}
    virtual ~Pet() = default;

    virtual void makeSound() const {
        std::cout << name_ << " is making a sound\n";
    }

    virtual void info() const {
        std::cout << "Pet: " << name_ << "\n";
    }

    void run() const {
        std::cout << name_ << " is running\n";
    }

    void wakeUp() const { makeSound(); }

    [[nodiscard]] std::string_view getName() const noexcept { return name_; }
};

class Cat final : public Pet {
public:
    explicit Cat(std::string_view name) : Pet{name} {}

    void makeSound() const override {
        std::cout << name_ << " — Meow meow meow...\n";
    }

    void info() const override {
        std::cout << "Cat: " << name_ << "\n";
    }
};

class Dog final : public Pet {
public:
    explicit Dog(std::string_view name) : Pet{name} {}

    void makeSound() const override {
        std::cout << name_ << " — Woof woof woof...\n";
    }

    void info() const override {
        std::cout << "Dog: " << name_ << "\n";
    }
};

class Parrot final : public Animal {
public:
    explicit Parrot(std::string_view name) : Animal{name} {}

    void makeSound() const override {
        std::cout << name_ << " — Caw caw caw...\n";
    }
};

class Friend;
void doIt(const class ClassObj&);

class ClassObj {
    friend class Friend;
    friend void doIt(const ClassObj&);

    int         data_ = 0;
    std::string name_ = "None";

    void print() const {
        std::cout << "Friends forever — " << data_ << " " << name_ << "\n";
    }
};

class Friend {
public:
    void doIt(ClassObj& c) {
        c.data_ = 2;
        c.name_ = "Friends";
        c.print();
    }
};

void doIt(const ClassObj& c) { c.print(); }

using PetVec = std::vector<std::unique_ptr<Pet>>;

void makeAllSound(const PetVec& pets) {
    std::cout << "\nAll pets making sound:\n";
    for (const auto& p : pets) { p->makeSound(); }
}

void showPetInfo(const PetVec& pets) {
    std::cout << "\nPet information:\n";
    for (const auto& p : pets) { p->info(); }
}

[[nodiscard]] std::string demangled_type_name(const std::type_info& info) {
    int status = 0;
    const std::unique_ptr<char, void (*)(void*)> demangled{
        abi::__cxa_demangle(info.name(), nullptr, nullptr, &status), std::free};
    return status == 0 ? std::string{demangled.get()} : std::string{info.name()};
}

void printType(const Pet& p) {
    std::cout << "Actual type: " << demangled_type_name(typeid(p)) << "\n";
}

const Pet* findPet(const PetVec& pets, std::string_view target) {
    auto it = std::ranges::find_if(pets, [target](const auto& p) {
        return p->getName() == target;
    });
    return it != pets.end() ? it->get() : nullptr;
}

void sortPets(PetVec& pets) {
    std::ranges::sort(pets, {}, &Pet::getName);
}

template <typename T>
concept Speaks = requires(const T& t) {
    { t.makeSound() } -> std::same_as<void>;
    { t.getName() } -> std::convertible_to<std::string_view>;
};

template <Speaks T>
void announce(const T& speaker) {
    std::cout << "[announce] " << speaker.getName() << " says: ";
    speaker.makeSound();
}

int main() {
    PetVec pets;
    pets.push_back(std::make_unique<Cat>("Tom"));
    pets.push_back(std::make_unique<Dog>("Huckelberry"));
    pets.push_back(std::make_unique<Cat>("Oreo"));

    pets[0]->run();
    pets[1]->makeSound();

    if (auto* cat = dynamic_cast<Cat*>(pets[0].get())) {
        std::cout << "dynamic_cast<Cat> succeeded: ";
        cat->makeSound();
    }

    if (auto* wrongCast = dynamic_cast<Cat*>(pets[1].get())) {
        wrongCast->makeSound();
    } else {
        std::cout << "dynamic_cast<Cat> on Dog — correctly failed\n";
    }

    auto parrot = std::make_unique<Parrot>("Jack");
    Animal* animal = parrot.get();
    animal->makeSound();
    parrot->wakeUp();

    ClassObj classObj;
    Friend   friendObj;
    friendObj.doIt(classObj);
    doIt(classObj);

    makeAllSound(pets);
    std::cout << "Total pets: " << pets.size() << "\n";

    for (const auto& p : pets) { printType(*p); }

    showPetInfo(pets);

    if (const Pet* found = findPet(pets, "Oreo")) {
        std::cout << "\nFound — ";
        found->makeSound();
    }

    std::cout << "\nSorted pets by name:\n";
    sortPets(pets);
    for (const auto& p : pets) { std::cout << p->getName() << "\n"; }

    std::cout << "\nErasing pets named 'Tom':\n";
    const auto removed = std::erase_if(pets, [](const auto& p) { return p->getName() == "Tom"; });
    std::cout << "removed " << removed << " pet(s), remaining: " << pets.size() << "\n";
    for (const auto& p : pets) { std::cout << p->getName() << "\n"; }

    std::cout << "\nSmart pointer demo:\n";
    auto smartPet = std::make_unique<Dog>("SmartDog");
    smartPet->makeSound();

    std::cout << "\nDuck typing across unrelated hierarchies (concept Speaks):\n";
    const Cat cat_value{"Whiskers"};
    const Parrot parrot_value{"Polly"};
    announce(cat_value);
    announce(parrot_value);

    assert(pets.size() == 2);

    return 0;
}
