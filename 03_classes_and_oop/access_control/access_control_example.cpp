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

    // ----------- NEW ADDITION -----------
public:
    int getPrivate() const {
        return priv_member;
    }
    // -----------------------------------
};

class Derived : public Base {
public:
    void access() {
        pub_member = 1;      // OK
        prot_member = 2;     // OK
        // priv_member = 3;  // Error
    }
};

class Friend {
public:
    void access(Base& obj) {
        obj.priv_member = 10;  // OK - friend class
    }
};

void friend_function() {
    Base obj;
    obj.priv_member = 20;  // OK - friend function
    obj.print();
}

// ----------- NEW ADDITIONS -----------

// Protected inheritance
class ProtectedDerived : protected Base {
public:
    void test() {
        pub_member = 5;   // becomes protected
        prot_member = 6;
    }
};

// Private inheritance
class PrivateDerived : private Base {
public:
    void test() {
        pub_member = 7;   // becomes private
        prot_member = 8;
    }
};

// Friend function with parameter
void friend_modify(Base& obj) {
    obj.priv_member = 99;
}

// ----------- EXTRA SMALL ADDITIONS -----------

// Utility to show object state clearly
void show_state(const Base& obj, const std::string& label) {
    std::cout << "[" << label << "] ";
    obj.print();
}

// Function demonstrating pass-by-reference vs value
void modify_public(Base& obj) {
    obj.pub_member += 5;
}

// ------------------------------------

int main() {
    Base obj;
    obj.pub_member = 5;  // OK
    // obj.prot_member = 1;  // Error
    // obj.priv_member = 2;  // Error

    obj.print();

    Derived d;
    d.access();           // modifies public & protected
    d.print();

    Friend f;
    f.access(obj);        // modifies private member
    obj.print();

    friend_function();    // friend function access

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

    // ----------------------------------

    return 0;
}
