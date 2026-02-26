#include <iostream>
using namespace std;

// Definition of a Super Class or Base Class
class Super {
  private:
    int storage; // private attribute
  protected:
    int accessible_storage; // protected attribute copy of storage
  public:
    Super() { storage = 0; accessible_storage = 0; } // default constructor
    Super(int val) { storage = accessible_storage = val; } // 🔹 added constructor

    void set(int val) { storage = accessible_storage = val; } // setter
    int get() { return storage; } // getter

    void reset() { storage = accessible_storage = 0; } // 🔹 added helper
};

// Definition of another Base class
class Base {
  protected:
    int storage;
  public:
    Base() { storage = 0; }
    Base(int val) { storage = val; } // 🔹 added constructor

    void set(int val) { storage = val; }
    int get() { return storage; }

    void reset() { storage = 0; } // 🔹 added helper
};

// Sub Class inheriting from the Super class (single inheritance)
class Sub : public Super {
  public:
    void print(void) {
      cout << " Storage = " << accessible_storage << endl;
    }
};

// Sub Class inheriting from multiple classes
class SubMulti : public Super, public Base {
  public:
    void print(void) {
      cout << " Storage Super = " << Super::accessible_storage << endl;
      cout << " Storage Base  = " << Base::storage << endl;
    }

    // 🔹 extra combined operation
    int totalStorage() {
      return Super::accessible_storage + Base::storage;
    }
};

int main(void) {

  SubMulti object;

  object.Super::set(100);
  object.Base::set(50);

  object.Super::set(object.Super::get() + 1);

  object.print();

  cout << " Total Storage = "
       << object.totalStorage() << endl; // 🔹 extra usage

  object.Super::reset(); // 🔹 reset demo
  object.Base::reset();

  cout << "\nAfter reset:\n";
  object.print();

  return 0;
}
