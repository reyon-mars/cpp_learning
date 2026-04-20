#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <memory> // ✅ ADDED for smart pointers
using namespace std;

class Pet {
protected:
    string name;
public:
    Pet(string name) : name(name) {}

    virtual ~Pet() {
        cout << "Destroying Pet: " << name << endl; // ✅ ADDED
    }

    void NameMe(string name) { this->name = name; }

    virtual void MakeSound(void) {
        cout << name << " says Objects " << endl;
    }

    // ----------- NEW ADDITION -----------
    virtual const char* getType() const {
        return "Pet";
    }

    // ✅ ADDED: Clone pattern (polymorphic copy)
    virtual Pet* clone() const {
        return new Pet(*this);
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

    const char* getType() const override {
        return "Dog";
    }

    // ✅ ADDED clone
    Pet* clone() const override {
        return new Dog(*this);
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

    const char* getType() const override {
        return "GermanShepherd";
    }

    // ✅ ADDED clone
    Pet* clone() const override {
        return new GermanShepherd(*this);
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

    const char* getType() const override {
        return "MastinEspanol";
    }

    // ✅ ADDED clone
    Pet* clone() const override {
        return new MastinEspanol(*this);
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

void printType(Pet& pet) {
    cout << "Actual type: " << typeid(pet).name() << endl;
}

void printTypeVirtual(const Pet& pet) {
    cout << "Virtual type: " << pet.getType() << endl;
}

int countDogs(const vector<Pet*>& pets) {
    int count = 0;
    for (auto p : pets) {
        if (dynamic_cast<Dog*>(p))
            count++;
    }
    return count;
}

// ✅ ADDED: smart pointer demo
void smartPointerDemo() {
    cout << "\nSmart Pointer Demo:\n";
    unique_ptr<Pet> pet = make_unique<GermanShepherd>("SmartDog");
    pet->MakeSound();
}

// ✅ ADDED: clone demo
void cloneDemo(const Pet& pet) {
    cout << "\nClone Demo:\n";
    unique_ptr<Pet> copy(pet.clone());
    copy->MakeSound();
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

    cout << "\nObject slicing example:\n";
    PlayWithPetByValue("SlicedDog", dog);

    cout << "\nSafePlay demo:\n";
    SafePlay(gs);
    SafePlay(mes);
    SafePlay(pet);

    cout << "\nRTTI types:\n";
    printType(pet);
    printType(dog);
    printType(gs);

    cout << "\nVirtual type info:\n";
    printTypeVirtual(gs);
    printTypeVirtual(mes);

    vector<Pet*> pets = { &pet, &dog, &gs, &mes };

    cout << "\nAll pets making sound:\n";
    for (const auto* p : pets) { // ✅ safer loop
        p->MakeSound();
    }

    cout << "Number of dogs: "
         << countDogs(pets) << endl;

    // -------- EXTRA NEW USAGE --------

    smartPointerDemo();
    cloneDemo(gs);

    // ----------------------------------

    delete petPointer;

    return 0;
}
