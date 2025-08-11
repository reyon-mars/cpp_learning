#include <iostream>
#include <string>
using namespace std;

class Pet {
  protected:
    string name;
  public:
    Pet( string name ) : name( name ) {}
    void NameMe( string name ) { this -> name = name;  return ; }
    virtual void MakeSound( void ) { cout << name << " says Objects " << endl; return ; }
};

//Declaring a child class of the Pet Superclass called dog
class Dog: public Pet {
  public:
    Dog( string name ) : Pet( name ) {}
    void MakeSound( void ) { cout << name << " says woof woof woof " << endl; return ; }
};

class GermanShepherd: public Dog {
  public:
    GermanShepherd( string name ) : Dog( name ) {}
    void MakeSound( void ) { cout << name << " says wuff wuff wuff " << endl; return ; }
	void Laufen( void ) { cout << name << " the german shepherd is running " << endl; return ; }
};

class MastinEspanol : public Dog {
  public:
    MastinEspanol( string name ) : Dog( name ) {}
    void MakeSound( void ) { cout << name << " says Guau Guau Guau " << endl; return ; }
	void Ejectuar( void ) { cout << name << " the Mastin Espanol is running " << endl; return ; }
};

void PlayWithPet( Pet &pet ) {
  GermanShepherd *gs;
  MastinEspanol *mes;
  if( ( gs = dynamic_cast<GermanShepherd * >( &pet ) ) ){ // using the dynamic_cast to check whether the Pet* can be converted to GermanShepherd *
    gs-> Laufen();
  }
  try{
    dynamic_cast<MastinEspanol &>(pet).Ejectuar(); // using dynamic_cast to try to convert the Pet reference to MastinEspanol reference
    }catch(...){} // if the casting fails catch the error and do nothing
  pet.MakeSound();
  return ;
}

void PlayWithPet( Pet *pet ){
  pet->MakeSound();
  return ;
}

// Declaring a function that takes an object by value
void PlayWithPetByValue( string name , Pet pet ){
  pet.NameMe( name );
  pet.MakeSound();
  return ;

}
// Declaring a functoin that takes an object as a pointer
void PlayWithPetByPointer( string name, Pet *pet ){
  pet -> NameMe( name );
  pet -> MakeSound();
  return ;
}
// Declaring a function that takes an object by reference
void PlayWithPetByReference( string name, Pet &pet ){
  pet.NameMe( name );
  pet.MakeSound();
}

int main( void ) {
  Pet petReference("Almonds") , *petPointer = new Pet("Max");
  PlayWithPetByPointer( "anonymous" , petPointer );
  PlayWithPetByReference( "anonymous" , petReference );
  PlayWithPetByPointer( "no_name", &petReference );
  PlayWithPetByReference( "no_name", *petPointer );

  Pet pet(" Animal " );
  Pet *pet_ptr = &pet;
  Dog dog(" Dog " );
  Dog *dog_ptr = &dog;
  GermanShepherd gs( " Hund " );
  GermanShepherd *gs_ptr = &gs;
  MastinEspanol mes( " Perro " );
  MastinEspanol *mes_ptr = &mes;
  PlayWithPet( pet );
  PlayWithPet( dog );
  PlayWithPet( gs );
  PlayWithPet( mes );
  PlayWithPet( pet_ptr );
  PlayWithPet( dog_ptr );
  PlayWithPet( gs_ptr );
  PlayWithPet( mes_ptr );

  return 0;

}