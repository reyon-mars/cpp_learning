#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <memory>     // ✅ ADDED
#include <algorithm>  // ✅ ADDED
#include <cassert>    // ✅ ADDED

using namespace std;

// ======================================================
// Animal base class
// ======================================================
class Animal {
protected:
    string name;

public:
    Animal(string name) : name(name) {
        MakeSound();
    }

    virtual ~Animal() { }

    virtual void MakeSound(void) {
        cout << name << " is making a sound " << endl;
    }

    void WakeUp(void) {
        cout << name << " just woke up ";
        MakeSound();
    }

    // ✅ ADDED
    string getName() const {
        return name;
    }
};

// ======================================================
// Pet base class
// ======================================================
class Pet {
protected:
    string name;

public:
    Pet(string n) : name(n) {}

    virtual ~Pet() { }

    void Run(void) {
        cout << name << " is running" << endl;
    }

    virtual void MakeSound(void) {
        cout << name << " is making sound " << endl;
    }

    void WakeUp(void) {
        MakeSound();
    }

    // ✅ ADDED
    virtual void Info() const {
        cout << "Pet name: " << name << endl;
    }

    // ✅ ADDED
    string getName() const {
        return name;
    }
};

// ======================================================
// Cat
// ======================================================
class Cat : public Pet {
public:
    Cat(string n) : Pet(n) {}

    void MakeSound(void) override {
        cout << Pet::name << " Meow meow meow... " << endl;
    }

    // ✅ ADDED
    void Info() const override {
        cout << "Cat object: " << name << endl;
    }
};

// ======================================================
// Dog
// ======================================================
class Dog : public Pet {
public:
    Dog(string n) : Pet(n) {}

    void MakeSound(void) override {
        cout << Pet::name << " Woof woof woof... " << endl;
    }

    // ✅ ADDED
    void Info() const override {
        cout << "Dog object: " << name << endl;
    }
};

// ======================================================
// Parrot
// ======================================================
class Parrot : public Animal {
public:
    Parrot(string n) : Animal(n) {}

    void MakeSound(void) override {
        cout << name << " Caw Caw Caw Caw... " << endl;
    }
};

// ======================================================
// Friend & Class example
// ======================================================
class Class {
private:
    friend class Friend;
    friend void DoIt(const Class& c);

    int data = 0;
    string name = "None";

    void print(void) const {
        cout << "Friend and Class are friends forever "
             << data << " " << name << endl;
    }
};

class Friend {
public:
    void DoIt(Class& c) {
        c.data = 2;
        c.name = "Friends";
        c.print();
    }
};

void DoIt(const Class& c) {
    c.print();
}

// ---------------- NEW ADDITIONS ----------------

// Polymorphic function
void makeAllSound(const vector<Pet*>& pets) {
    cout << "\nAll pets making sound:\n";
    for (const auto& p : pets) {
        p->MakeSound();
    }
}

// RTTI demo
void printType(Pet* p) {
    cout << "Actual type: " << typeid(*p).name() << endl;
}

// Count pets
int countPets(const vector<Pet*>& pets) {
    return pets.size();
}

// Safe delete helper
void cleanup(vector<Pet*>& pets) {
    for (auto p : pets) {
        delete p;
    }
    pets.clear();
}

// ---------------- EXTRA SMALL ADDITIONS ----------------

// Print all pet info
void showPetInfo(const vector<Pet*>& pets) {
    cout << "\nPet information:\n";
    for (const auto& p : pets) {
        p->Info();
    }
}

// Find pet by name
Pet* findPet(const vector<Pet*>& pets, const string& target) {
    for (auto p : pets) {
        if (p->getName() == target) {
            return p;
        }
    }
    return nullptr;
}

// Sort pets by name
void sortPets(vector<Pet*>& pets) {
    sort(pets.begin(), pets.end(),
        [](Pet* a, Pet* b) {
            return a->getName() < b->getName();
        });
}

// Smart pointer demo
void smartPointerDemo() {
    unique_ptr<Pet> smartPet = make_unique<Dog>("SmartDog");

    cout << "\nSmart pointer demo:\n";
    smartPet->MakeSound();
}

// ------------------------------------------------

// ======================================================
// MAIN
// ======================================================
int main(void) {

    Pet* pet1 = new Cat("Tom");
    Pet* pet2 = new Dog("Huckelberry");

    pet1->Run();
    pet2->MakeSound();

    static_cast<Cat*>(pet1)->MakeSound();

    if (Cat* safeCat = dynamic_cast<Cat*>(pet1)) {
        cout << "dynamic_cast successful: ";
        safeCat->MakeSound();
    }

    // Failed dynamic_cast
    if (Cat* wrongCast = dynamic_cast<Cat*>(pet2)) {
        wrongCast->MakeSound();
    } else {
        cout << "dynamic_cast failed (Dog is not Cat)\n";
    }

    Cat* cat1 = new Cat("Oreo");
    static_cast<Pet*>(cat1)->MakeSound();

    Parrot* parrot;
    Animal* animal1 = parrot = new Parrot("Jack");

    animal1->MakeSound();
    parrot->WakeUp();

    Class class_object;
    Friend friend_object;

    friend_object.DoIt(class_object);
    DoIt(class_object);

    // -------- NEW POLYMORPHISM USAGE --------
    vector<Pet*> pets = { pet1, pet2, cat1 };

    makeAllSound(pets);

    cout << "Total pets: " << countPets(pets) << endl;

    printType(pet1);
    printType(pet2);

    // -------- EXTRA FEATURE USAGE --------

    showPetInfo(pets);

    Pet* found = findPet(pets, "Oreo");
    if (found) {
        cout << "\nFound pet: ";
        found->MakeSound();
    }

    cout << "\nSorted pets by name:\n";
    sortPets(pets);

    for (const auto& p : pets) {
        cout << p->getName() << endl;
    }

    smartPointerDemo();

    // ✅ ADDED assertion
    assert(countPets(pets) == 3);

    // ---------------------------------------

    // Cleanup pets safely
    cleanup(pets);

    delete parrot;

    return 0;
}
