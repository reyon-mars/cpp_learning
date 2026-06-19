#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <memory>      for smart pointers
#include <algorithm>  
#include <cassert>    

using namespace std;

class Pet {
protected:
    string name;

public:
    Pet(string name)
        : name(name) {}

    virtual ~Pet() {
        cout << "Destroying Pet: "
             << name << endl;
    }

    void NameMe(string name) {
        this->name = name;
    }

    virtual void MakeSound(void) {
        cout << name
             << " says Objects "
             << endl;
    }

    // ----------- NEW ADDITION -----------
    virtual const char* getType() const {
        return "Pet";
    }

    : Clone pattern
    virtual Pet* clone() const {
        return new Pet(*this);
    }

    : info function
    virtual void Info() const {
        cout << "Pet name: "
             << name << endl;
    }

    : getter
    string getName() const {
        return name;
    }
    // -----------------------------------
};


// Declaring a child class of the Pet Superclass called Dog
class Dog : public Pet {
public:
    Dog(string name)
        : Pet(name) {}

    void MakeSound(void) override {
        cout << name
             << " says woof woof woof "
             << endl;
    }

    const char* getType() const override {
        return "Dog";
    }

     clone
    Pet* clone() const override {
        return new Dog(*this);
    }

     info
    void Info() const override {
        cout << "Dog named "
             << name << endl;
    }
};


class GermanShepherd : public Dog {
public:
    GermanShepherd(string name)
        : Dog(name) {}

    void MakeSound(void) override {
        cout << name
             << " says wuff wuff wuff "
             << endl;
    }

    void Laufen(void) {
        cout << name
             << " the german shepherd is running "
             << endl;
    }

    const char* getType() const override {
        return "GermanShepherd";
    }

     clone
    Pet* clone() const override {
        return new GermanShepherd(*this);
    }

     info
    void Info() const override {
        cout << "German Shepherd: "
             << name << endl;
    }
};


class MastinEspanol : public Dog {
public:
    MastinEspanol(string name)
        : Dog(name) {}

    void MakeSound(void) override {
        cout << name
             << " says Guau Guau Guau "
             << endl;
    }

    void Ejectuar(void) {
        cout << name
             << " the Mastin Espanol is running "
             << endl;
    }

    const char* getType() const override {
        return "MastinEspanol";
    }

     clone
    Pet* clone() const override {
        return new MastinEspanol(*this);
    }

     info
    void Info() const override {
        cout << "Mastin Espanol: "
             << name << endl;
    }
};


void PlayWithPet(Pet &pet) {

    GermanShepherd *gs;
    MastinEspanol *mes;

    if ((gs = dynamic_cast<GermanShepherd*>(&pet))) {
        gs->Laufen();
    }

    try {
        dynamic_cast<MastinEspanol&>(pet)
            .Ejectuar();
    }
    catch (...) {}

    pet.MakeSound();
}


void PlayWithPet(Pet *pet) {
    if (!pet) return;

    pet->MakeSound();
}


void PlayWithPetByValue(
    string name,
    Pet pet) {

    pet.NameMe(name);
    pet.MakeSound();
}


void PlayWithPetByPointer(
    string name,
    Pet *pet) {

    pet->NameMe(name);
    pet->MakeSound();
}


void PlayWithPetByReference(
    string name,
    Pet &pet) {

    pet.NameMe(name);
    pet.MakeSound();
}


// ----------- NEW ADDITIONS -----------

void SafePlay(Pet& pet) {

    if (auto gs =
        dynamic_cast<GermanShepherd*>(&pet)) {

        gs->Laufen();
    }
    else if (auto mes =
        dynamic_cast<MastinEspanol*>(&pet)) {

        mes->Ejectuar();
    }
    else {
        cout << "Unknown pet type\n";
    }
}


void printType(Pet& pet) {
    cout << "Actual type: "
         << typeid(pet).name()
         << endl;
}


void printTypeVirtual(
    const Pet& pet) {

    cout << "Virtual type: "
         << pet.getType()
         << endl;
}


int countDogs(
    const vector<Pet*>& pets) {

    int count = 0;

    for (auto p : pets) {

        if (dynamic_cast<Dog*>(p))
            count++;
    }

    return count;
}


: smart pointer demo
void smartPointerDemo() {

    cout << "\nSmart Pointer Demo:\n";

    unique_ptr<Pet> pet =
        make_unique<GermanShepherd>("SmartDog");

    pet->MakeSound();
}


: clone demo
void cloneDemo(const Pet& pet) {

    cout << "\nClone Demo:\n";

    unique_ptr<Pet> copy(pet.clone());

    copy->MakeSound();
}


// ----------- EXTRA ADDITIONS -----------

// Print all pet info
void printAllPets(
    const vector<Pet*>& pets) {

    cout << "\nAll Pet Info:\n";

    for (const auto* p : pets) {
        p->Info();
    }
}


// Find pet by name
Pet* findPetByName(
    const vector<Pet*>& pets,
    const string& target) {

    for (auto* p : pets) {

        if (p->getName() == target)
            return p;
    }

    return nullptr;
}


// Count specific type
int countGermanShepherds(
    const vector<Pet*>& pets) {

    int count = 0;

    for (auto* p : pets) {

        if (dynamic_cast<GermanShepherd*>(p))
            count++;
    }

    return count;
}


// Clone all pets
vector<unique_ptr<Pet>>
cloneAllPets(
    const vector<Pet*>& pets) {

    vector<unique_ptr<Pet>> copies;

    for (auto* p : pets) {
        copies.emplace_back(p->clone());
    }

    return copies;
}


// Sort pet names
void printSortedNames(
    const vector<Pet*>& pets) {

    vector<string> names;

    for (auto* p : pets) {
        names.push_back(p->getName());
    }

    sort(names.begin(), names.end());

    cout << "\nSorted Pet Names:\n";

    for (const auto& n : names) {
        cout << n << endl;
    }
}

// ------------------------------------


// ======================================================
// MAIN
// ======================================================

int main(void) {

    Pet petReference("Almonds"),
        *petPointer = new Pet("Max");

    PlayWithPetByPointer(
        "anonymous",
        petPointer);

    PlayWithPetByReference(
        "anonymous",
        petReference);

    PlayWithPetByPointer(
        "no_name",
        &petReference);

    PlayWithPetByReference(
        "no_name",
        *petPointer);

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

    PlayWithPetByValue(
        "SlicedDog",
        dog);

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

    vector<Pet*> pets = {
        &pet,
        &dog,
        &gs,
        &mes
    };

    assert(!pets.empty()); 

    cout << "\nAll pets making sound:\n";

    for (const auto* p : pets) {
        p->MakeSound();
    }

    cout << "Number of dogs: "
         << countDogs(pets)
         << endl;

    // -------- EXTRA NEW USAGE --------

    smartPointerDemo();

    cloneDemo(gs);

    // -------- MORE EXTRA USAGE --------

    printAllPets(pets);

    cout << "\nSearching for pet 'Hund':\n";

    Pet* found =
        findPetByName(pets, "Hund");

    if (found) {
        cout << "Found pet type: "
             << found->getType()
             << endl;
    }

    cout << "\nGerman Shepherd count: "
         << countGermanShepherds(pets)
         << endl;

    auto clonedPets =
        cloneAllPets(pets);

    cout << "\nCloned pets making sound:\n";

    for (const auto& cp : clonedPets) {
        cp->MakeSound();
    }

    printSortedNames(pets);

    // ----------------------------------

    delete petPointer;

    return 0;
}
