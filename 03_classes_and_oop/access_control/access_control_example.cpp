// Access Control Exercise
// public, private, protected, friend

#include <iostream>

class Base {
public:
    int pub_member = 0;

    void print() const {
        std::cout << "pub: " << pub_member
                  << " prot: " << prot_member
                  << " priv: " << priv_member << "\n";
    }
    
protected:
    int prot_member = 0;
    
private:
    int priv_member = 0;
    
    friend class Friend;
    friend void friend_function();

public:
    int getPrivate() const {
        return priv_member;
    }

    // ----------- EXTRA ADDITION -----------
    void setPrivate(int val) {
        priv_member = val;
    }
    // -------------------------------------
};

class Derived : public Base {
public:
    void access() {
        pub_member = 1;      
        prot_member = 2;     
    }
};

class Friend {
public:
    void access(Base& obj) {
        obj.priv_member = 10;
    }
};

void friend_function() {
    Base obj;
    obj.priv_member = 20;
    obj.print();
}

// ----------- NEW ADDITIONS -----------

class ProtectedDerived : protected Base {
public:
    void test() {
        pub_member = 5;
        prot_member = 6;
    }
};

class PrivateDerived : private Base {
public:
    void test() {
        pub_member = 7;
        prot_member = 8;
    }
};

void friend_modify(Base& obj) {
    obj.priv_member = 99;
}

// ----------- EXTRA SMALL ADDITIONS -----------

void show_state(const Base& obj, const std::string& label) {
    std::cout << "[" << label << "] ";
    obj.print();
}

void modify_public(Base& obj) {
    obj.pub_member += 5;
}

// ----------- MORE ADVANCED ADDITIONS -----------

// Static member demo
class Counter {
public:
    static int count;

    Counter() {
        count++;
    }
};

int Counter::count = 0;

// Virtual function demo
class Shape {
public:
    virtual void draw() {
        std::cout << "Drawing Shape\n";
    }
};

class Circle : public Shape {
public:
    void draw() override {
        std::cout << "Drawing Circle\n";
    }
};

// Object slicing demo
void slicing_demo(Base obj) {
    std::cout << "Slicing demo (Base copy): ";
    obj.print();
}

// ------------------------------------

int main() {
    Base obj;
    obj.pub_member = 5;

    obj.print();

    Derived d;
    d.access();
    d.print();

    Friend f;
    f.access(obj);
    obj.print();

    friend_function();

    // -------- NEW FEATURE USAGE --------

    ProtectedDerived pd;
    pd.test();

    PrivateDerived pr;
    pr.test();

    friend_modify(obj);
    std::cout << "After friend_modify: ";
    obj.print();

    std::cout << "Access via getter: "
              << obj.getPrivate() << "\n";

    // -------- EXTRA USAGE --------

    show_state(obj, "Current State");

    modify_public(obj);
    show_state(obj, "After modify_public");

    // -------- MORE ADVANCED USAGE --------

    std::cout << "\n--- Static Member Demo ---\n";
    Counter c1, c2, c3;
    std::cout << "Objects created: " << Counter::count << "\n";

    std::cout << "\n--- Polymorphism Demo ---\n";
    Shape* s = new Circle();
    s->draw();   // runtime polymorphism
    delete s;

    std::cout << "\n--- Object Slicing Demo ---\n";
    slicing_demo(d);  // Derived → Base copy

    std::cout << "\n--- Setter Demo ---\n";
    obj.setPrivate(123);
    obj.print();

    // ----------------------------------

    return 0;
}
