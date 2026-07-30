#include <iostream>
#include <memory>
#include <vector>
#include <typeinfo>
#include <utility>
#include <cassert>
#include <concepts>
#include <span>
#include <ranges>
#include <algorithm>
#include <mutex>
#include <thread>
#include <format>
#include <variant>
#include <expected>
#include <string>

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
    [[nodiscard]] virtual std::unique_ptr<Animal> clone() const = 0;
};

template <typename Derived>
class Cloneable : public Animal {
public:
    using Animal::Animal;

    [[nodiscard]] std::unique_ptr<Animal> clone() const override {
        return std::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};

class Dog : public Cloneable<Dog> {
public:
    Dog()                     { std::cout << "Dog constructed\n"; }
    Dog(const Dog&)           { std::cout << "Dog copied\n";      }
    Dog(Dog&&) noexcept       { std::cout << "Dog moved\n";       }
    ~Dog() override           { std::cout << "Dog destroyed\n";   }

    void speak() const override { std::cout << "Woof!\n";         }
    void info()  const override { std::cout << "This is a Dog\n"; }
};

class Cat : public Cloneable<Cat> {
public:
    Cat()                     { std::cout << "Cat constructed\n"; }
    Cat(const Cat&)           { std::cout << "Cat copied\n";      }
    Cat(Cat&&) noexcept       { std::cout << "Cat moved\n";       }
    ~Cat() override           { std::cout << "Cat destroyed\n";   }

    void speak() const override { std::cout << "Meow!\n";         }
    void info()  const override { std::cout << "This is a Cat\n"; }
};

class Bird final : public Cloneable<Bird> {
public:
    Bird()          { std::cout << "Bird constructed\n"; }
    ~Bird() override{ std::cout << "Bird destroyed\n";   }

    void speak() const override { std::cout << "Chirp!\n";          }
    void info()  const override { std::cout << "This is a Bird\n";  }
};

class Fish final : public Cloneable<Fish> {
public:
    Fish()          { std::cout << "Fish constructed\n"; }
    ~Fish() override{ std::cout << "Fish destroyed\n";   }

    void speak() const override { std::cout << "Blub!\n";          }
    void info()  const override { std::cout << "This is a Fish\n"; }
};

template <typename T>
concept AnimalLike = std::derived_from<T, Animal>;

template <AnimalLike T, typename... Args>
[[nodiscard]] std::unique_ptr<Animal> makeAnimal(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

enum class Kind { Dog, Cat, Bird, Fish };

[[nodiscard]] std::expected<std::unique_ptr<Animal>, std::string> createByKind(Kind kind) {
    switch (kind) {
        case Kind::Dog:  return makeAnimal<::Dog>();
        case Kind::Cat:  return makeAnimal<::Cat>();
        case Kind::Bird: return makeAnimal<::Bird>();
        case Kind::Fish: return makeAnimal<::Fish>();
    }
    return std::unexpected(std::format("Unknown Kind value: {}", std::to_underlying(kind)));
}

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

void speakAll(std::span<const std::unique_ptr<Animal>> group) {
    std::ranges::for_each(group, [](const auto& a) { a->speak(); });
}

void concurrentSpeakDemo(std::span<const std::unique_ptr<Animal>> group) {
    std::mutex ioMutex;
    std::vector<std::jthread> workers;
    workers.reserve(group.size());
    for (const auto& a : group) {
        workers.emplace_back([&ioMutex, ptr = a.get()] {
            std::scoped_lock lock{ioMutex};
            ptr->speak();
        });
    }
}

template <typename... Ts>
struct Overloaded : Ts... { using Ts::operator()...; };
template <typename... Ts>
Overloaded(Ts...) -> Overloaded<Ts...>;

void variantDemo() {
    using AnimalVariant = std::variant<Dog, Cat, Bird, Fish>;
    std::vector<AnimalVariant> zoo;
    zoo.emplace_back(Dog{});
    zoo.emplace_back(Cat{});
    zoo.emplace_back(Bird{});
    zoo.emplace_back(Fish{});

    for (auto& v : zoo) {
        std::visit(Overloaded{
            [](const Dog& d)  { d.speak(); },
            [](const Cat& c)  { c.speak(); },
            [](const Bird& b) { b.speak(); },
            [](const Fish& f) { f.speak(); }
        }, v);
    }
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
    Dog placeholder;
}

int main() {
    std::cout << "--- No slicing possible: Animal is abstract ---\n";
    Dog dog;

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
    animals.reserve(4);
    animals.push_back(std::make_unique<Dog>());
    animals.push_back(std::make_unique<Cat>());
    animals.push_back(std::make_unique<Bird>());
    animals.push_back(std::make_unique<Fish>());
    std::ranges::for_each(animals, [](const auto& a) { a->speak(); });

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
    Fish fish;
    for (const Animal* a : {static_cast<const Animal*>(&dog),
                             static_cast<const Animal*>(&cat),
                             static_cast<const Animal*>(&bird),
                             static_cast<const Animal*>(&fish)}) {
        a->info();
    }

    std::cout << "\n--- Rvalue reference ---\n";
    consumeAnimal(Dog{});

    moveDemo();
    ownershipTransfer();

    std::cout << "\n--- Vector info() ---\n";
    std::ranges::for_each(animals, [](const auto& a) { a->info(); });

    std::cout << "\n--- Clone (prototype pattern) ---\n";
    auto clonedDog = dog.clone();
    clonedDog->speak();

    std::cout << "\n--- Concept-constrained factory ---\n";
    auto conceptCat = makeAnimal<Cat>();
    conceptCat->speak();

    std::cout << "\n--- std::expected factory ---\n";
    for (auto kind : {Kind::Dog, Kind::Cat, Kind::Bird, Kind::Fish}) {
        if (auto result = createByKind(kind); result.has_value()) {
            result.value()->info();
        } else {
            std::cout << result.error() << "\n";
        }
    }

    std::cout << "\n--- std::span speakAll ---\n";
    speakAll(animals);

    std::cout << "\n--- Concurrent speak (jthread) ---\n";
    concurrentSpeakDemo(animals);

    std::cout << "\n--- std::variant / std::visit ---\n";
    variantDemo();

    std::cout << "\n--- End of main ---\n";
    return 0;
}
