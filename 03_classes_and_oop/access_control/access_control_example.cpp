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

    return 0;
}
