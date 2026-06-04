#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <utility>
#include <cassert>

class Animal {
public:
    Animal()                       { std::cout << "Animal constructed\n"; }
    Animal(const Animal&)          { std::cout << "Animal copied\n";      }
    Animal(Animal&&) noexcept      { std::cout << "Animal moved\n";       }
    virtual ~Animal()              { std::cout << "Animal destroyed\n";   }

    Animal& operator=(const Animal&)     = default;
    Animal& operator=(Animal&&) noexcept = default;

    virtual void speak() const { std::cout << "...\n";            }
    virtual void info()  const { std::cout << "Generic Animal\n"; }
};

class Dog : public Animal {
public:
    Dog()                     { std::cout << "Dog constructed\n"; }
    Dog(const Dog&)           { std::cout << "Dog copied\n";      }
    Dog(Dog&&) noexcept       { std::cout << "Dog moved\n";       }
    ~Dog() override           { std::cout << "Dog destroyed\n";   }

    void speak() const override { std::cout << "Woof!\n";         }
    void info()  const override { std::cout << "This is a Dog\n"; }
};

class Cat : public Animal {
public:
    Cat()                     { std::cout << "Cat constructed\n"; }
    Cat(const Cat&)           { std::cout << "Cat copied\n";      }
    Cat(Cat&&) noexcept       { std::cout << "Cat moved\n";       }
    ~Cat() override           { std::cout << "Cat destroyed\n";   }

    void speak() const override { std::cout << "Meow!\n";         }
    void info()  const override { std::cout << "This is a Cat\n"; }
};

class Bird final : public Animal {
public:
    Bird()          { std::cout << "Bird constructed\n"; }
    ~Bird() override{ std::cout << "Bird destroyed\n";   }

    void speak() const override { std::cout << "Chirp!\n";          }
    void info()  const override { std::cout << "This is a Bird\n";  }
};

void makeSpeak(const Animal& a) { a.speak(); }

[[nodiscard]] std::unique_ptr<Animal> createAnimal(bool isDog) {
    return isDog ? std::unique_ptr<Animal>{std::make_unique<Dog>()}
                 : std::unique_ptr<Animal>{std::make_unique<Cat>()};
}

void printType(const Animal& a) {
    std::cout << "RTTI: " << typeid(a).name() << "\n";
}

void checkDog(const Animal& a) {
    if (const auto* d = dynamic_cast<const Dog*>(&a)) {
        std::cout << "dynamic_cast<Dog> succeeded: ";
        d->speak();
    } else {
        std::cout << "dynamic_cast<Dog> failed\n";
    }
}

void consumeAnimal(Animal&& a) {
    std::cout << "Consuming rvalue animal: ";
    a.speak();
}

void moveDemo() {
    std::cout << "\n--- Move Demo ---\n";
    Dog d1;
    Dog d2 = std::move(d1);
    d2.speak();
}

void ownershipTransfer() {
    std::cout << "\n--- Ownership Transfer ---\n";
    auto ptr1 = std::make_unique<Dog>();
    assert(ptr1 != nullptr);
    auto ptr2 = std::move(ptr1);
    std::cout << "ptr1 null after move: " << std::boolalpha << (ptr1 == nullptr) << "\n";
    ptr2->speak();
}

void lifetimeDemo() {
    std::cout << "\n--- Lifetime Tracker ---\n";
    Animal a;
}

int main() {
    std::cout << "--- Object slicing ---\n";
    Dog dog;
    Animal sliced = dog;
    sliced.speak();

    std::cout << "\n--- Slicing via by-value lambda ---\n";
    auto byValue = [](Animal a) { a.speak(); };
    byValue(dog);

    std::cout << "\n--- Reference (no slicing) ---\n";
    Animal& ref = dog;
    ref.speak();

    std::cout << "\n--- Pointer (no slicing) ---\n";
    Animal* ptr = &dog;
    ptr->speak();

    std::cout << "\n--- unique_ptr (best practice) ---\n";
    auto smartDog = std::make_unique<Dog>();
    smartDog->speak();

    std::cout << "\n--- Polymorphic vector ---\n";
    std::vector<std::unique_ptr<Animal>> animals;
    animals.reserve(3);
    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());
    animals.push_back(std::make_unique<Bird>());
    for (const auto& a : animals) { a->speak(); }

    std::cout << "\n--- makeSpeak (by reference) ---\n";
    makeSpeak(dog);

    std::cout << "\n--- Smart factory ---\n";
    auto f1 = createAnimal(true);
    auto f2 = createAnimal(false);
    f1->speak();
    f2->speak();

    lifetimeDemo();

    std::cout << "\n--- RTTI ---\n";
    Cat cat;
    printType(dog);
    printType(cat);

    std::cout << "\n--- dynamic_cast ---\n";
    checkDog(dog);
    checkDog(cat);

    std::cout << "\n--- Virtual info() ---\n";
    Bird bird;
    for (const Animal* a : {static_cast<const Animal*>(&dog),
                             static_cast<const Animal*>(&cat),
                             static_cast<const Animal*>(&bird)}) {
        a->info();
    }

    std::cout << "\n--- Rvalue reference ---\n";
    consumeAnimal(Dog{});

    moveDemo();
    ownershipTransfer();

    std::cout << "\n--- Vector info() ---\n";
    for (const auto& a : animals) { a->info(); }

    std::cout << "\n--- End of main ---\n";
    return 0;
}
