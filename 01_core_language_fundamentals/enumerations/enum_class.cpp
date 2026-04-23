#include <iostream>
#include <vector>

// ----------------  ENUMS ----------------

enum class Animal {
    Cat,
    Dog,
    Bird,
    Fish
};

enum class Vehicle {
    Car,
    Bike,
    Bus,
    Train
};

enum class Food {
    Pizza,
    Burger,
    Pasta
};

// ----------------  HELPERS ----------------

// Validate Animal from int
bool isValidAnimal(int value) {
    return value >= static_cast<int>(Animal::Cat) &&
           value <= static_cast<int>(Animal::Fish);
}

// to_string for Animal
const char* enumName(Animal a) {
    switch (a) {
        case Animal::Cat: return "Cat";
        case Animal::Dog: return "Dog";
        case Animal::Bird: return "Bird";
        case Animal::Fish: return "Fish";
    }
    return "Unknown";
}


// to_string for Vehicle
const char* enumName(Vehicle v) {
    switch (v) {
        case Vehicle::Car: return "Car";
        case Vehicle::Bike: return "Bike";
        case Vehicle::Bus: return "Bus";
        case Vehicle::Train: return "Train";
    }
    return "Unknown";
}

// to_string for Food
const char* enumName(Food f) {
    switch (f) {
        case Food::Pizza: return "Pizza";
        case Food::Burger: return "Burger";
        case Food::Pasta: return "Pasta";
    }
    return "Unknown";
}

// Safe conversion: int → Animal
bool intToAnimal(int value, Animal& out) {
    if (!isValidAnimal(value)) return false;
    out = static_cast<Animal>(value);
    return true;
}

// Iterate all Animal values
std::vector<Animal> getAllAnimals() {
    return { Animal::Cat, Animal::Dog, Animal::Bird, Animal::Fish };
}

// Compare two enums
template <typename T>
bool areSame(T a, T b) {
    return a == b;
}

// enum → int helper
template <typename T>
int toInt(T e) {
    return static_cast<int>(e);
}

// ------------------------------------------------


// ================= MAIN =================

int main() {

    Animal a = Animal::Dog;
    Vehicle v = Vehicle::Car;
    Food f = Food::Pizza;

    std::cout << "Basic Enum Usage:\n";
    std::cout << "Animal: " << enumName(a) << "\n";

    // ================= ADD IN MAIN =================

    std::cout << "\nAdvanced Enum Features:\n";

    std::cout << "Vehicle name: " << enumName(v) << "\n";
    std::cout << "Food name: " << enumName(f) << "\n";

    Animal converted;
    if (intToAnimal(1, converted)) {
        std::cout << "Converted from int 1 → " << enumName(converted) << "\n";
    }

    std::cout << "Iterating all animals:\n";
    for (auto animal : getAllAnimals()) {
        std::cout << enumName(animal) << "\n";
    }

    std::cout << "Are Cat and Dog same? "
              << (areSame(Animal::Cat, Animal::Dog) ? "Yes\n" : "No\n");

    std::cout << "Animal (Dog) as int: " << toInt(a) << "\n";

    return 0;
}
