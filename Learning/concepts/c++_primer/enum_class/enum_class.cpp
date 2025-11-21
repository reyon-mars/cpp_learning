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

int main( void ){
    

    Animal a = Animal::Cat;
    Vehicle v  = Vehicle::Car;

    describe( a );
    describe( v );

    return 0;
    
}