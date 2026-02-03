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
// SMALL EXTRA CODE (ADDED ONLY)
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

// Print enum numeric value
template <typename T>
void printEnumValue(T e) {
    std::cout << "Enum numeric value: "
              << static_cast<int>(e) << '\n';
}

// Describe all items in a container
template <typename T>
void describeAll(const std::vector<T>& items) {
    for (const auto& i : items) {
        describe(i);
        printEnumValue(i);
    }
}

// ---- VERY SMALL EXTRA HELPERS ----

// Count items in enum vector
template <typename T>
std::size_t countItems(const std::vector<T>& items) {
    return items.size();
}

// Simple divider
void printDivider() {
    std::cout << "----------------------\n";
}

// ---- NEW SMALL ADDITIONS ----

// Check if enum value is zero
template <typename T>
bool isFirstEnumValue(T e) {
    return static_cast<int>(e) == 0;
}

// Print vector numeric values only
template <typename T>
void printEnumNumbers(const std::vector<T>& items) {
    std::cout << "Enum numbers: ";
    for (const auto& i : items) {
        std::cout << static_cast<int>(i) << " ";
    }
    std::cout << "\n";
}

// ---------------------------------

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

    std::cout << (isFirstEnumValue(a) ? "First enum value\n"
                                      : "Not first enum value\n");

    printDivider();

    // Small extra demo
    Food f = Food::Pizza;
    describe(f);
    printEnumValue(f);

    printDivider();

    // ---- very small extra usage ----
    std::vector<Animal> animals = {
        Animal::Cat,
        Animal::Dog,
        Animal::Bird
    };

    std::cout << "All animals:\n";
    describeAll(animals);

    std::cout << "Total animals: "
              << countItems(animals) << '\n';

    printEnumNumbers(animals);

    return 0;
}
