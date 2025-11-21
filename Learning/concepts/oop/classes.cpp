#include <iostream>
#include <string>
using namespace std;

// Defining a base class called Animal
class Animal{
  protected:
    string name;

  public:
    Animal( string name ) : name(name){ MakeSound();}
    virtual void MakeSound( void ) { cout << name << " is Making a sound " << endl; } // defining a virtual function
    void WakeUp( void ) { cout << name << " just woke up "; MakeSound(); return ; } // invoking the virtual method from within the class
};

// Defining a base class called Pet
class Pet {
  protected:
    string name; // creating a protected attribute to allow inheritance but keeping the data hidden

  public:
    Pet(string n) : name(n) {}; // Defining a constructor to initialize the name attribute during the object creation
    void Run( void ){ cout << name << " is running" << endl; } // Run Action of the pet
    void MakeSound( void ) { cout << name << " is making sound " << endl; }
    void WakeUp( void ) { MakeSound(); return; }
};

// Creating a child class called Cat , inheriting from the base parent class Pet with public visibility
class Cat: public Pet {
  public:
    Cat( string n) : Pet(n) {}; // constructor of the child class to initialize the attribute of the parent class
    void MakeSound( void ) { cout << Pet::name <<" Meow meow meow... " << endl; return ; } // the original function in the Pet super class is overridden
};

class Dog: public Pet {
  public:
    Dog( string n ) : Pet(n) {};
    void MakeSound( void ) { cout << Pet::name <<  " Woof woof woof... " << endl; return ; }
};
// Creating a child class inheriting from the Animal class called Parrot
class Parrot: public Animal{
  protected:
    string name;
  public:
    Parrot( string n ) : Animal(n), name(n) {};
    void MakeSound( void ) { cout << name << " Caw Caw Caw Caw... " << endl; return ; } // polymorphing the MakeSound function
};

class Class{
  private:
    friend class Friend; // declaring that the Friend class is a friend
    friend void DoIt( const Class &c ); // declaring a friend function
    int data;
    string name;

  private:
    void print( void ) const{ cout << "Friend and Class are friends forever " << data << name << endl; }
};

class Friend{
  public:
    void DoIt( Class &c ) {
      c.data = 2; // accessing the private of the Class class
      c.name = "Friends";
      c.print();
    }
};
// friend function of the Class class
void DoIt( const Class &c ){
  c.print();
}

int main( void ) {
  
  Pet* pet1 = new Cat("Tom");
  Pet* pet2 = new Dog("Huckelberry");
  Cat* cat1 = new Cat("Oreo");

  Pet *pet3, *pet4;
  Dog* dog1;
  Cat* cat2;
  pet3 = dog1 = new Dog("Max");
  pet4 = cat2 = new Cat("Garfield");
  pet1 -> Run();
  pet2 -> MakeSound();
  static_cast< Cat* > ( pet1 ) -> MakeSound(); // Example of static_cast to temporarily change the Pet* to Cat*
  pet2 -> Run();
  static_cast< Dog* > ( pet1 ) -> MakeSound(); // Misusing the static_cast to point to incompatible object using pointer of another type
  cat1 -> MakeSound();
  static_cast< Pet * > ( cat1 ) -> MakeSound(); // using the static_cast to reverse the overriding of the MakeSound() method
  dog1 -> MakeSound();
  pet3 -> MakeSound();
  pet4 -> MakeSound();
  cat2 -> MakeSound();

  Parrot* parrot;
  Animal* animal1 = parrot = new Parrot("Jack");
  animal1 -> MakeSound();
  parrot -> MakeSound();
  parrot -> WakeUp();

  Class class_object;
  Friend friend_object;

  friend_object.DoIt( class_object );
  DoIt( class_object );

  return 0;
}
