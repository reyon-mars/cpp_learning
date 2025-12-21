#include <iostream>
#include <cstdint>

// ======================================================
// ORIGINAL ENUMS
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
// ORIGINAL DESCRIBE FUNCTIONS
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
        case Vehicle::Bus:
            std::cout << "Bus goes honk honk\n";
            break;
        case Vehicle::Bike:
            std::cout << "Bike goes ring ring\n";
            break;
    }
}

// ======================================================
// EXTRA ENUMS + FUNCTIONS
// ======================================================

enum class Food : uint8_t {
    Pizza,
    Burger,
    Sushi
};

void describeFood(Food f) {
    switch (f) {
        case Food::Pizza:
            std::cout << "Pizza is cheesy!\n";
            break;
        case Food::Burger:
            std::cout << "Burger is juicy!\n";
            break;
        case Food::Sushi:
            std::cout << "Sushi is fresh!\n";
            break;
    }
}

enum class Color : uint8_t {
    Red,
    Green,
    Blue
};

void describeColor(Color c) {
    switch (c) {
        case Color::Red:
            std::cout << "Red is vibrant!\n";
            break;
        case Color::Green:
            std::cout << "Green is peaceful!\n";
            break;
        case Color::Blue:
            std::cout << "Blue is calming!\n";
            break;
    }
}

enum class Planet : uint8_t {
    Earth,
    Mars,
    Jupiter
};

void describePlanet(Planet p) {
    switch (p) {
        case Planet::Earth:
            std::cout << "Earth: our home planet!\n";
            break;
        case Planet::Mars:
            std::cout << "Mars: the red planet!\n";
            break;
        case Planet::Jupiter:
            std::cout << "Jupiter: the gas giant!\n";
            break;
    }
}

enum class Weather : uint8_t {
    Sunny,
    Rainy,
    Snowy
};

void describeWeather(Weather w) {
    switch (w) {
        case Weather::Sunny:
            std::cout << "Weather is sunny ☀️\n";
            break;
        case Weather::Rainy:
            std::cout << "Weather is rainy 🌧️\n";
            break;
        case Weather::Snowy:
            std::cout << "Weather is snowy ❄️\n";
            break;
    }
}

// ======================================================
// TEMPLATE + RAII
// ======================================================

template<typename T>
void genericDescribe(T value) {
    std::cout << "Generic enum value = "
              << static_cast<int>(value) << '\n';
}

struct ScopedAnnounce {
    explicit ScopedAnnounce(const char* msg) {
        std::cout << "[ENTER] " << msg << '\n';
    }
    ~ScopedAnnounce() {
        std::cout << "[EXIT] Scope ended\n";
    }
};

// ======================================================
// MAIN
// ======================================================

int main(void) {

    // ---------- ORIGINAL LOGIC ----------
    Animal a = Animal::Cat;
    Vehicle v = Vehicle::Car;

    describe(a);
    describe(v);

    // ---------- EXTRA ENUM DEMOS ----------
    std::cout << "\n[Food Demo]\n";
    describeFood(Food::Pizza);

    std::cout << "\n[Color Demo]\n";
    describeColor(Color::Blue);

    std::cout << "\n[Planet Demo]\n";
    describePlanet(Planet::Mars);

    std::cout << "\n[Weather Demo]\n";
    describeWeather(Weather::Sunny);
    genericDescribe(Weather::Rainy);

    // ---------- RAII + COMPOSITION ----------
    {
        ScopedAnnounce scope("Final enum demo");
        describePlanet(Planet::Earth);
        describe(Animal::Dog);
        describe(Vehicle::Bike);
    }

    return 0;
}
