#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
using namespace std;

class Pet {
protected:
    string name;
public:
    Pet(string name) : name(name) {}

    virtual ~Pet() {}

    void NameMe(string name) { this->name = name; }

    virtual void MakeSound(void) {
        cout << name << " says Objects " << endl;
    }

    // ----------- NEW ADDITION -----------
    virtual const char* getType() const {
        return "Pet";
    }
    // -----------------------------------
};

// Declaring a child class of the Pet Superclass called Dog
class Dog : public Pet {
public:
    Dog(string name) : Pet(name) {}

    void MakeSound(void) override {
        cout << name << " says woof woof woof " << endl;
    }

    // ----------- NEW ADDITION -----------
    const char* getType() const override {
        return "Dog";
    }
    // -----------------------------------
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

    // ----------- NEW ADDITION -----------
    const char* getType() const override {
        return "GermanShepherd";
    }
    // -----------------------------------
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

    // ----------- NEW ADDITION -----------
    const char* getType() const override {
        return "MastinEspanol";
    }
    // -----------------------------------
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
    if (!pet) return;

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

// ----------- NEW ADDITIONS -----------

// Safer casting version (no exceptions)
void SafePlay(Pet& pet) {
    if (auto gs = dynamic_cast<GermanShepherd*>(&pet)) {
        gs->Laufen();
    }
    else if (auto mes = dynamic_cast<MastinEspanol*>(&pet)) {
        mes->Ejectuar();
    }
    else {
        cout << "Unknown pet type\n";
    }
}

// RTTI
void printType(Pet& pet) {
    cout << "Actual type: " << typeid(pet).name() << endl;
}

// Print type using virtual function
void printTypeVirtual(const Pet& pet) {
    cout << "Virtual type: " << pet.getType() << endl;
}

// Count how many dogs in container
int countDogs(const vector<Pet*>& pets) {
    int count = 0;
    for (auto p : pets) {
        if (dynamic_cast<Dog*>(p))
            count++;
    }
    return count;
}

// ------------------------------------

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

    // -------- NEW FEATURE USAGE --------

    // Object slicing demo
    cout << "\nObject slicing example:\n";
    PlayWithPetByValue("SlicedDog", dog);

    // Safer casting
    cout << "\nSafePlay demo:\n";
    SafePlay(gs);
    SafePlay(mes);
    SafePlay(pet);

    // RTTI demo
    cout << "\nRTTI types:\n";
    printType(pet);
    printType(dog);
    printType(gs);

    // Virtual type info
    cout << "\nVirtual type info:\n";
    printTypeVirtual(gs);
    printTypeVirtual(mes);

    // Polymorphic container
    vector<Pet*> pets = { &pet, &dog, &gs, &mes };
    cout << "\nAll pets making sound:\n";
    for (auto p : pets) {
        p->MakeSound();
    }

    // Count dogs
    cout << "Number of dogs: "
         << countDogs(pets) << endl;

    // ----------------------------------

    delete petPointer;

    return 0;
}
