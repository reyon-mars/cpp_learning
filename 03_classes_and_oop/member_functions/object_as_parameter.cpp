#include <iostream>
#include <string>
using namespace std;

class Pet {
protected:
    string name;
public:
    Pet(string name) : name(name) {}

    virtual ~Pet() {}   // 🔹 added virtual destructor

    void NameMe(string name) { this->name = name; }

    virtual void MakeSound(void) {
        cout << name << " says Objects " << endl;
    }
};

// Declaring a child class of the Pet Superclass called Dog
class Dog : public Pet {
public:
    Dog(string name) : Pet(name) {}

    void MakeSound(void) override {   // 🔹 added override
        cout << name << " says woof woof woof " << endl;
    }
};

class GermanShepherd : public Dog {
public:
    GermanShepherd(string name) : Dog(name) {}

    void MakeSound(void) override {
        cout << name << " says wuff wuff wuff " << endl;
    }

    void Laufen(void) {
        cout << name << " the german shepherd is running " << endl;
    }
};

class MastinEspanol : public Dog {
public:
    MastinEspanol(string name) : Dog(name) {}

    void MakeSound(void) override {
        cout << name << " says Guau Guau Guau " << endl;
    }

    void Ejectuar(void) {
        cout << name << " the Mastin Espanol is running " << endl;
    }
};

void PlayWithPet(Pet &pet) {
    GermanShepherd *gs;
    MastinEspanol *mes;

    if ((gs = dynamic_cast<GermanShepherd*>(&pet))) {
        gs->Laufen();
    }

    try {
        dynamic_cast<MastinEspanol&>(pet).Ejectuar();
    }
    catch (...) {}

    pet.MakeSound();
}

void PlayWithPet(Pet *pet) {
    if (!pet) return;   // 🔹 small safety check

    pet->MakeSound();
}

void PlayWithPetByValue(string name, Pet pet) {
    pet.NameMe(name);
    pet.MakeSound();
}

void PlayWithPetByPointer(string name, Pet *pet) {
    pet->NameMe(name);
    pet->MakeSound();
}

void PlayWithPetByReference(string name, Pet &pet) {
    pet.NameMe(name);
    pet.MakeSound();
}

int main(void) {

    Pet petReference("Almonds"),
        *petPointer = new Pet("Max");

    PlayWithPetByPointer("anonymous", petPointer);
    PlayWithPetByReference("anonymous", petReference);
    PlayWithPetByPointer("no_name", &petReference);
    PlayWithPetByReference("no_name", *petPointer);

    Pet pet("Animal");
    Pet *pet_ptr = &pet;

    Dog dog("Dog");
    Dog *dog_ptr = &dog;

    GermanShepherd gs("Hund");
    GermanShepherd *gs_ptr = &gs;

    MastinEspanol mes("Perro");
    MastinEspanol *mes_ptr = &mes;

    PlayWithPet(pet);
    PlayWithPet(dog);
    PlayWithPet(gs);
    PlayWithPet(mes);

    PlayWithPet(pet_ptr);
    PlayWithPet(dog_ptr);
    PlayWithPet(gs_ptr);
    PlayWithPet(mes_ptr);

    delete petPointer;   // 🔹 cleanup

    return 0;
}
