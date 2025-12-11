#include <iostream>

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

void describe( Animal a ){
    switch( a ){
        case Animal::Cat:
        std::cout << "Cat goes meow meow" << '\n';
        break;

        case Animal::Dog:
        std::cout << "Dog goes bark bark" << '\n';
        break;

        case Animal::Bird:
        std::cout << "Bird goes chirp chirp" << '\n';
        break;
    }
}

void describe( Vehicle v ){
    switch( v ){
        case Vehicle::Car:
        std::cout << "Car goes vroom"<< '\n';
        break;

        case Vehicle::Bus:
        std::cout << "Car goes honk honk" << '\n';
        break;

        case Vehicle::Bike:
        std::cout << "Bike goes ring ring"  << '\n';
        break;
    }
}

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
                std::cout << "Pizza is cheesy!" << '\n';
                break;
            case Food::Burger:
                std::cout << "Burger is juicy!" << '\n';
                break;
            case Food::Sushi:
                std::cout << "Sushi is fresh!" << '\n';
                break;
        }
    };

    Food myFood = Food::Pizza;
    describe(myFood);

    // ---------------------------------
    // MORE EXTRA ADDED CODE (NEW PART)
    // ---------------------------------

    enum class Color : u_int8_t {
        Red,
        Green,
        Blue
    };

    auto describeColor = [](Color c){
        switch(c){
            case Color::Red:
                std::cout << "Red is vibrant!" << '\n';
                break;
            case Color::Green:
                std::cout << "Green is peaceful!" << '\n';
                break;
            case Color::Blue:
                std::cout << "Blue is calming!" << '\n';
                break;
        }
    };

    Color favorite = Color::Blue;
    describeColor(favorite);

    return 0;
}

// ======================================================
// EVEN MORE EXTRA CODE (appended without changing original)
// ======================================================

enum class Planet : u_int8_t {
    Earth,
    Mars,
    Jupiter
};

void describePlanet(Planet p) {
    switch(p) {
        case Planet::Earth:
            std::cout << "Earth: our home planet!" << '\n';
            break;
        case Planet::Mars:
            std::cout << "Mars: the red planet!" << '\n';
            break;
        case Planet::Jupiter:
            std::cout << "Jupiter: the gas giant!" << '\n';
            break;
    }
}

struct BonusStruct {
    int data = 99;
};

BonusStruct globalBonus;

int runAfterMain = [](){
    std::cout << "=== Extra global code running after main() ===\n";
    describePlanet(Planet::Mars);
    std::cout << "BonusStruct.data = " << globalBonus.data << '\n';
    return 0;
}();
