#include <iostream>
#include <cstdint>
#include <vector>

// ======================================================
// ORIGINAL ENUMS (UNCHANGED)
// ======================================================

enum class Animal : uint8_t {
    Cat,
    Dog,
    Bird
};

enum class Vehicle : uint8_t {
    Car,
    Bike,
    Bus
};

// ======================================================
// ORIGINAL DESCRIBE FUNCTIONS (UNCHANGED)
// ======================================================

void describe(Animal a) {
    switch (a) {
        case Animal::Cat:
            std::cout << "Cat goes meow meow\n";
            break;
        case Animal::Dog:
            std::cout << "Dog goes bark bark\n";
            break;
        case Animal::Bird:
            std::cout << "Bird goes chirp chirp\n";
            break;
    }
}

void describe(Vehicle v) {
    switch (v) {
        case Vehicle::Car:
            std::cout << "Car goes vroom\n";
            break;
        case Vehicle::Bike:
            std::cout << "Bike goes ring ring\n";
            break;
        case Vehicle::Bus:
            std::cout << "Bus goes honk honk\n";
            break;
    }
}

// ======================================================
// SMALL EXTRA CODE (ADDITION ONLY)
// ======================================================

enum class Food : uint8_t {
    Pizza,
    Burger
};

void describe(Food f) {
    switch (f) {
        case Food::Pizza:
            std::cout << "Pizza is tasty!\n";
            break;
        case Food::Burger:
            std::cout << "Burger is yummy!\n";
            break;
    }
}

// ---- very small helper addition ----
template <typename T>
void printEnumValue(T e) {
    std::cout << "Enum numeric value: "
              << static_cast<int>(e) << '\n';
}

// ---- tiny extra helper ----
template <typename T>
void describeAll(const std::vector<T>& items) {
    for (const auto& i : items) {
        describe(i);
        printEnumValue(i);
    }
}

// ======================================================
// MAIN
// ======================================================

int main(void) {

    // Original usage
    Animal a = Animal::Cat;
    Vehicle v = Vehicle::Car;

    describe(a);
    describe(v);

    printEnumValue(a);
    printEnumValue(v);

    // Small extra demo
    Food f = Food::Pizza;
    describe(f);
    printEnumValue(f);

    // ---- very small extra usage ----
    std::vector<Animal> animals = {
        Animal::Cat,
        Animal::Dog,
        Animal::Bird
    };

    std::cout << "\nAll animals:\n";
    describeAll(animals);

    return 0;
}
