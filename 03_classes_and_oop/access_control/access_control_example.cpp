// Access Control Exercise
// public, private, protected, friend

#include <iostream>

class Base {
public:
    int pub_member = 0;
    
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
}

int main() {
    Base obj;
    obj.pub_member = 5;  // OK
    // obj.prot_member;  // Error
    // obj.priv_member;  // Error
    
    return 0;
}
