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

: Extra enum
enum class Color {
    Red,
    Green,
    Blue
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

: to_string for Color
const char* enumName(Color c) {
    switch (c) {
        case Color::Red: return "Red";
        case Color::Green: return "Green";
        case Color::Blue: return "Blue";
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
    return {
        Animal::Cat,
        Animal::Dog,
        Animal::Bird,
        Animal::Fish
    };
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

: Print divider
void printDivider() {
    std::cout << "----------------------\n";
}

: Count total animals
size_t animalCount() {
    return getAllAnimals().size();
}

: Print all vehicles
void printVehicles() {

    std::vector<Vehicle> vehicles = {
        Vehicle::Car,
        Vehicle::Bike,
        Vehicle::Bus,
        Vehicle::Train
    };

    for (auto vehicle : vehicles) {
        std::cout << enumName(vehicle) << "\n";
    }
}

// ------------------------------------------------
// ✅ NEW ADDITIONS

std::vector<Color> getAllColors() {
    return {
        Color::Red,
        Color::Green,
        Color::Blue
    };
}

size_t vehicleCount() {
    return 4;
}

size_t foodCount() {
    return 3;
}

void printColors() {
    for (auto color : getAllColors()) {
        std::cout << enumName(color) << "\n";
    }
}

template<typename T>
void printEnumValue(const char* label, T value) {
    std::cout << label << ": "
              << static_cast<int>(value)
              << "\n";
}

// ------------------------------------------------


// ================= MAIN =================

int main() {

    Animal a = Animal::Dog;
    Vehicle v = Vehicle::Car;
    Food f = Food::Pizza;

    
    Color c = Color::Green;

    std::cout << "Basic Enum Usage:\n";

    std::cout << "Animal: "
              << enumName(a) << "\n";

    // ================= ADD IN MAIN =================

    printDivider();

    std::cout << "\nAdvanced Enum Features:\n";

    std::cout << "Vehicle name: "
              << enumName(v) << "\n";

    std::cout << "Food name: "
              << enumName(f) << "\n";

    
    std::cout << "Color name: "
              << enumName(c) << "\n";

    Animal converted;

    if (intToAnimal(1, converted)) {

        std::cout << "Converted from int 1 → "
                  << enumName(converted) << "\n";
    }

    printDivider();

    std::cout << "Iterating all animals:\n";

    for (auto animal : getAllAnimals()) {
        std::cout << enumName(animal) << "\n";
    }

    printDivider();

    std::cout << "All vehicles:\n";
    printVehicles();

    printDivider();

    // ✅ NEW
    std::cout << "All colors:\n";
    printColors();

    printDivider();

    std::cout << "Are Cat and Dog same? "
              << (areSame(Animal::Cat, Animal::Dog)
                  ? "Yes\n"
                  : "No\n");

    
    std::cout << "Are Car and Car same? "
              << (areSame(Vehicle::Car, Vehicle::Car)
                  ? "Yes\n"
                  : "No\n");

    printDivider();

    std::cout << "Animal (Dog) as int: "
              << toInt(a) << "\n";

    
    std::cout << "Vehicle (Car) as int: "
              << toInt(v) << "\n";

    std::cout << "Food (Pizza) as int: "
              << toInt(f) << "\n";

    std::cout << "Color (Green) as int: "
              << toInt(c) << "\n";

    printDivider();

    
    std::cout << "Total animals available: "
              << animalCount() << "\n";

    // ✅ NEW
    std::cout << "Total vehicles available: "
              << vehicleCount() << "\n";

    std::cout << "Total foods available: "
              << foodCount() << "\n";

    printDivider();

    
    std::cout << "Animal validation test:\n";

    std::cout << "Is 2 valid? "
              << (isValidAnimal(2) ? "Yes" : "No")
              << "\n";

    std::cout << "Is 10 valid? "
              << (isValidAnimal(10) ? "Yes" : "No")
              << "\n";

    printDivider();

    // ✅ NEW
    std::cout << "Generic enum integer values:\n";

    printEnumValue("Animal::Dog", a);
    printEnumValue("Vehicle::Car", v);
    printEnumValue("Food::Pizza", f);
    printEnumValue("Color::Green", c);

    printDivider();

    // ✅ NEW
    std::cout << "Enum Statistics Summary:\n";
    std::cout << "- Animals: " << animalCount() << "\n";
    std::cout << "- Vehicles: " << vehicleCount() << "\n";
    std::cout << "- Foods: " << foodCount() << "\n";
    std::cout << "- Colors: " << getAllColors().size() << "\n";

    return 0;
}
