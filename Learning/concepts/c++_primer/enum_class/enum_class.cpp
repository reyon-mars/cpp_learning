#include <iostream>
#include <cstdint>

// ======================================================
// ORIGINAL ENUMS
// ======================================================

enum class Animal : u_int8_t{ 
    Cat,
    Dog,
    Bird
};

enum class Vehicle : u_int8_t{
    Car, 
    Bike,
    Bus
};

// ======================================================
// ORIGINAL DESCRIBE FUNCTIONS
// ======================================================

void describe( Animal a ){
    switch( a ){
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

void describe( Vehicle v ){
    switch( v ){
        case Vehicle::Car:
            std::cout << "Car goes vroom\n";
            break;
        case Vehicle::Bus:
            std::cout << "Car goes honk honk\n";
            break;
        case Vehicle::Bike:
            std::cout << "Bike goes ring ring\n";
            break;
    }
}

// ======================================================
// MAIN (UNCHANGED LOGIC)
// ======================================================

int main( void )
{    
    Animal a = Animal::Cat;
    Vehicle v  = Vehicle::Car;

    describe( a );
    describe( v );

    // -----------------------------
    // EXTRA ADDED CODE BELOW
    // -----------------------------

    enum class Food : u_int8_t {
        Pizza,
        Burger,
        Sushi
    };

    auto describe = [](Food f){
        switch(f){
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
    };

    Food myFood = Food::Pizza;
    describe(myFood);

    // ---------------------------------
    // MORE EXTRA ADDED CODE
    // ---------------------------------

    enum class Color : u_int8_t {
        Red,
        Green,
        Blue
    };

    auto describeColor = [](Color c){
        switch(c){
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
    };

    Color favorite = Color::Blue;
    describeColor(favorite);

    return 0;
}

// ======================================================
// EVEN MORE EXTRA CODE (BEFORE MAIN)
// ======================================================

enum class Planet : u_int8_t {
    Earth,
    Mars,
    Jupiter
};

void describePlanet(Planet p) {
    switch(p) {
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

struct BonusStruct {
    int data = 99;
};

BonusStruct globalBonus;

int runBeforeMain = [](){
    std::cout << "=== Extra global code running before main() ===\n";
    describePlanet(Planet::Mars);
    std::cout << "BonusStruct.data = " << globalBonus.data << '\n';
    return 0;
}();

// ======================================================
// TEMPLATE + GENERIC ENUM HANDLING
// ======================================================

template<typename T>
void genericDescribe(T value) {
    std::cout << "Generic describe value = "
              << static_cast<int>(value) << '\n';
}

enum class Weather : u_int8_t {
    Sunny,
    Rainy,
    Snowy
};

void describeWeather(Weather w) {
    switch(w) {
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

int __extra2 = [](){
    std::cout << "=== Weather system initialized ===\n";
    describeWeather(Weather::Sunny);
    genericDescribe(Weather::Rainy);
    return 0;
}();

// ======================================================
// RAII + ENUM COMPOSITION
// ======================================================

struct ScopedAnnounce {
    ScopedAnnounce(const char* msg) {
        std::cout << "[ENTER] " << msg << '\n';
    }
    ~ScopedAnnounce() {
        std::cout << "[EXIT] Scope ended\n";
    }
};

int __extra3 = [](){
    ScopedAnnounce s("Final enum demo");
    describePlanet(Planet::Earth);
    describe(Animal::Dog);
    describe(Vehicle::Bike);
    return 0;
}();
