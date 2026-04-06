// ----------- MORE ADVANCED ADDITIONS -----------

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

// ------------------------------------------------


// ================= ADD IN MAIN =================

// (Add near the end before return)

std::cout << "\nAdvanced Enum Features:\n";

// ✅ ADDED: to_string for other enums
std::cout << "Vehicle name: " << enumName(v) << "\n";
std::cout << "Food name: " << enumName(f) << "\n";

// ✅ ADDED: safe int → enum
Animal converted;
if (intToAnimal(1, converted)) {
    std::cout << "Converted from int 1 → " << enumName(converted) << "\n";
}

// ✅ ADDED: iterate all animals
std::cout << "Iterating all animals:\n";
for (auto animal : getAllAnimals()) {
    std::cout << enumName(animal) << "\n";
}

// ✅ ADDED: comparison helper
std::cout << "Are Cat and Dog same? "
          << (areSame(Animal::Cat, Animal::Dog) ? "Yes\n" : "No\n");
