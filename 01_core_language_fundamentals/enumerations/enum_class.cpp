#include <iostream>
#include <cstdint>
#include <vector>

// ======================================================
// ENUMS 
// ======================================================

enum class Animal : uint8_t {
    Cat,
    Dog,
    Bird,
    Fish          // ✅ small addition
};

enum class Vehicle : uint8_t {
    Car,
    Bike,
    Bus,
    Train         // ✅ small addition
};

enum class Food : uint8_t {
    Pizza,
    Burger,
    Pasta         // ✅ small addition
};

// ======================================================
// DESCRIBE FUNCTIONS
// ======================================================

void describe(Animal a) {
    switch (a) {
        case Animal::Cat:   std::cout << "Cat goes meow meow\n"; break;
        case Animal::Dog:   std::cout << "Dog goes bark bark\n"; break;
        case Animal::Bird:  std::cout << "Bird goes chirp chirp\n"; break;
        case Animal::Fish:  std::cout << "Fish goes blub blub\n"; break;
    }
}

void describe(Vehicle v) {
    switch (v) {
        case Vehicle::Car:   std::cout << "Car goes vroom\n"; break;
        case Vehicle::Bike:  std::cout << "Bike goes ring ring\n"; break;
        case Vehicle::Bus:   std::cout << "Bus goes honk honk\n"; break;
        case Vehicle::Train: std::cout << "Train goes choo choo\n"; break;
    }
}

void describe(Food f) {
    switch (f) {
        case Food::Pizza:  std::cout << "Pizza is tasty!\n"; break;
        case Food::Burger: std::cout << "Burger is yummy!\n"; break;
        case Food::Pasta:  std::cout << "Pasta is delicious!\n"; break;
    }
}

// ======================================================
// SMALL HELPERS
// ======================================================

// Print enum numeric value
template <typename T>
void printEnumValue(T e) {
    std::cout << "Enum numeric value: "
              << static_cast<int>(e) << '\n';
}

// Convert enum to name (tiny utility)
template <typename T>
const char* enumName(T) {
    return "Unknown";
}

template <>
const char* enumName(Animal a) {
    switch (a) {
        case Animal::Cat: return "Cat";
        case Animal::Dog: return "Dog";
        case Animal::Bird: return "Bird";
        case Animal::Fish: return "Fish";
    }
    return "Unknown";
}

// Describe all items in a container
template <typename T>
void describeAll(const std::vector<T>& items) {
    for (const auto& i : items) {
        describe(i);
        printEnumValue(i);
    }
}

// Count items
template <typename T>
std::size_t countItems(const std::vector<T>& items) {
    return items.size();
}

void printDivider() {
    std::cout << "----------------------\n";
}

// Check if enum value is zero
template <typename T>
bool isFirstEnumValue(T e) {
    return static_cast<int>(e) == 0;
}

// Print vector numeric values
template <typename T>
void printEnumNumbers(const std::vector<T>& items) {
    std::cout << "Enum numbers: ";
    for (const auto& i : items)
        std::cout << static_cast<int>(i) << " ";
    std::cout << "\n";
}

// ======================================================
// MAIN
// ======================================================

int main(void) {

    Animal a = Animal::Cat;
    Vehicle v = Vehicle::Train;
    Food f = Food::Pasta;

    describe(a);
    describe(v);
    describe(f);

    printEnumValue(a);
    printEnumValue(v);
    printEnumValue(f);

    std::cout << "Animal name: " << enumName(a) << "\n";

    std::cout << (isFirstEnumValue(a) ? "First enum value\n"
                                      : "Not first enum value\n");

    printDivider();

    std::vector<Animal> animals = {
        Animal::Cat,
        Animal::Dog,
        Animal::Bird,
        Animal::Fish
    };

    std::cout << "All animals:\n";
    describeAll(animals);

    std::cout << "Total animals: "
              << countItems(animals) << '\n';

    printEnumNumbers(animals);

    return 0;
}
