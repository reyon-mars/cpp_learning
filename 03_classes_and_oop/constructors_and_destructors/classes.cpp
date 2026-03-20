#include <iostream>
#include <string>
using namespace std;

// ======================================================
// Animal base class
// ======================================================
class Animal {#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
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

    // -------- NEW ADDITION (failed dynamic_cast) --------
    if (Cat* wrongCast = dynamic_cast<Cat*>(pet2)) {
        wrongCast->MakeSound();
    } else {
        cout << "dynamic_cast failed (Dog is not Cat)\n";
    }
    // ---------------------------------------------------

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

    printType(pet1);
    printType(pet2);
    // ---------------------------------------

    delete pet1;
    delete pet2;
    delete cat1;
    delete parrot;

    return 0;
}
protected:
    string name;

public:
    Animal(string name) : name(name) {
        MakeSound();
    }

    virtual ~Animal() { }   // 🔹 added virtual destructor

    virtual void MakeSound(void) {
        cout << name << " is making a sound " << endl;
    }

    void WakeUp(void) {
        cout << name << " just woke up ";
        MakeSound();
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

    virtual ~Pet() { }   // 🔹 added virtual destructor

    void Run(void) {
        cout << name << " is running" << endl;
    }

    virtual void MakeSound(void) {   // 🔹 made virtual
        cout << name << " is making sound " << endl;
    }

    void WakeUp(void) {
        MakeSound();
    }
};

// ======================================================
// Cat
// ======================================================
class Cat : public Pet {
public:
    Cat(string n) : Pet(n) {}

    void MakeSound(void) override {   // 🔹 override keyword
        cout << Pet::name << " Meow meow meow... " << endl;
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

// ======================================================
// MAIN
// ======================================================
int main(void) {

    Pet* pet1 = new Cat("Tom");
    Pet* pet2 = new Dog("Huckelberry");

    pet1->Run();
    pet2->MakeSound();

    // 🔹 unsafe static_cast (kept intentionally)
    static_cast<Cat*>(pet1)->MakeSound();

    // 🔹 safe dynamic_cast example
    if (Cat* safeCat = dynamic_cast<Cat*>(pet1)) {
        cout << "dynamic_cast successful: ";
        safeCat->MakeSound();
    }

    Cat* cat1 = new Cat("Oreo");
    static_cast<Pet*>(cat1)->MakeSound(); // reverse overriding

    Parrot* parrot;
    Animal* animal1 = parrot = new Parrot("Jack");

    animal1->MakeSound();   // polymorphism
    parrot->WakeUp();

    Class class_object;
    Friend friend_object;

    friend_object.DoIt(class_object);
    DoIt(class_object);

    // 🔹 cleanup
    delete pet1;
    delete pet2;
    delete cat1;
    delete parrot;

    return 0;
}
