#include <iostream>
using namespace std;

// Definition of a Super Class or Base Class
class Super {
  private:
    int storage; // private attribute
  protected:
    int accessible_storage; // protected attribute copy of storage
  public:
    Super() { storage = 0; accessible_storage = 0; }
    Super(int val) { storage = accessible_storage = val; }

    void set(int val) { storage = accessible_storage = val; }
    int get() { return storage; }

    void reset() { storage = accessible_storage = 0; }

    // -------- NEW ADDITION --------
    virtual void info() {
        cout << "Super class\n";
    }
    // --------------------------------
};

// Definition of another Base class
class Base {
  protected:
    int storage;
  public:
    Base() { storage = 0; }
    Base(int val) { storage = val; }

    void set(int val) { storage = val; }
    int get() { return storage; }

    void reset() { storage = 0; }

    // -------- NEW ADDITION --------
    virtual void info() {
        cout << "Base class\n";
    }
    // --------------------------------
};

// Sub Class inheriting from the Super class (single inheritance)
class Sub : public Super {
  public:
    void print(void) {
      cout << " Storage = " << accessible_storage << endl;
    }

    // -------- EXTRA ADDITION --------
    void increment() {
        accessible_storage++;
    }
    // --------------------------------
};

// Sub Class inheriting from multiple classes
class SubMulti : public Super, public Base {
  public:

    // -------- NEW ADDITION (constructor chaining) --------
    SubMulti(int a, int b) : Super(a), Base(b) {}
    SubMulti() {}
    // ----------------------------------------------------

    void print(void) {
      cout << " Storage Super = " << Super::accessible_storage << endl;
      cout << " Storage Base  = " << Base::storage << endl;
    }

    // 🔹 extra combined operation
    int totalStorage() {
      return Super::accessible_storage + Base::storage;
    }

    // -------- NEW ADDITION --------
    void setBoth(int a, int b) {
        Super::set(a);
        Base::set(b);
    }

    void showInfo() {
        Super::info();
        Base::info();
    }
    // --------------------------------

    // -------- EXTRA ADDITIONS --------

    void incrementBoth() {
        Super::accessible_storage++;
        Base::storage++;
    }

    void printAddresses() {
        cout << "Address Super part: " << static_cast<Super*>(this) << endl;
        cout << "Address Base part : " << static_cast<Base*>(this) << endl;
    }

    // --------------------------------
};

int main(void) {

  SubMulti object;

  object.Super::set(100);
  object.Base::set(50);

  object.Super::set(object.Super::get() + 1);

  object.print();

  cout << " Total Storage = "
       << object.totalStorage() << endl;

  object.Super::reset();
  object.Base::reset();

  cout << "\nAfter reset:\n";
  object.print();

  // -------- NEW FEATURE USAGE --------

  SubMulti obj2(10, 20);
  obj2.print();

  obj2.setBoth(30, 40);
  cout << "After setBoth:\n";
  obj2.print();

  // Ambiguity resolution
  cout << "Super get: " << obj2.Super::get() << endl;
  cout << "Base get: " << obj2.Base::get() << endl;

  // Virtual function demo
  obj2.showInfo();

  // -------- EXTRA USAGE --------

  obj2.incrementBoth();
  cout << "After incrementBoth:\n";
  obj2.print();

  obj2.printAddresses();

  // ----------------------------------

  return 0;
}
