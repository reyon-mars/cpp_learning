#include <iostream>
using namespace std;

// Definition of a Super Class or Base Class
class Super {
  private:
    int storage;
  protected:
    int accessible_storage;
  public:
    Super() { storage = 0; accessible_storage = 0; }
    Super(int val) { storage = accessible_storage = val; }

    void set(int val) { storage = accessible_storage = val; }
    int get() const { return storage; } // ✅ const added

    void reset() { storage = accessible_storage = 0; }

    virtual void info() {
        cout << "Super class\n";
    }

    // -------- EXTRA ADDITION --------
    virtual ~Super() {
        cout << "Super destructor\n";
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
    int get() const { return storage; } // ✅ const added

    void reset() { storage = 0; }

    virtual void info() {
        cout << "Base class\n";
    }

    // -------- EXTRA ADDITION --------
    virtual ~Base() {
        cout << "Base destructor\n";
    }
    // --------------------------------
};

// Sub Class inheriting from the Super class
class Sub : public Super {
  public:
    void print(void) {
      cout << " Storage = " << accessible_storage << endl;
    }

    void increment() {
        accessible_storage++;
    }

    // -------- NEW ADDITION --------
    void info() override {
        cout << "Sub class (derived from Super)\n";
    }
    // --------------------------------
};

// Multiple inheritance
class SubMulti : public Super, public Base {
  public:

    SubMulti(int a, int b) : Super(a), Base(b) {}
    SubMulti() {}

    void print(void) {
      cout << " Storage Super = " << Super::accessible_storage << endl;
      cout << " Storage Base  = " << Base::storage << endl;
    }

    int totalStorage() {
      return Super::accessible_storage + Base::storage;
    }

    void setBoth(int a, int b) {
        Super::set(a);
        Base::set(b);
    }

    void showInfo() {
        Super::info();
        Base::info();
    }

    void incrementBoth() {
        Super::accessible_storage++;
        Base::storage++;
    }

    void printAddresses() {
        cout << "Address Super part: " << static_cast<Super*>(this) << endl;
        cout << "Address Base part : " << static_cast<Base*>(this) << endl;
    }

    // -------- EXTRA ADDITION --------
    void info() {
        cout << "SubMulti class (multiple inheritance)\n";
    }
    // --------------------------------
};

// -------- EXTRA GLOBAL ADDITIONS --------

// Polymorphism demo
void polymorphism_demo(Super* ptr) {
    cout << "Polymorphism call: ";
    ptr->info();  // virtual dispatch
}

// Type identification (RTTI)
void type_check(Super* ptr) {
    cout << "Type check: ";
    if (dynamic_cast<Sub*>(ptr)) {
        cout << "Pointer is of type Sub\n";
    } else {
        cout << "Pointer is NOT Sub\n";
    }
}

// ---------------------------------------

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

  // -------- ORIGINAL USAGE --------

  SubMulti obj2(10, 20);
  obj2.print();

  obj2.setBoth(30, 40);
  cout << "After setBoth:\n";
  obj2.print();

  cout << "Super get: " << obj2.Super::get() << endl;
  cout << "Base get: " << obj2.Base::get() << endl;

  obj2.showInfo();

  // -------- EXTRA USAGE --------

  obj2.incrementBoth();
  cout << "After incrementBoth:\n";
  obj2.print();

  obj2.printAddresses();

  // -------- NEW ADVANCED DEMOS --------

  cout << "\n--- Polymorphism Demo ---\n";
  Sub s;
  polymorphism_demo(&s);

  cout << "\n--- Type Check Demo ---\n";
  type_check(&s);

  cout << "\n--- Direct info() call ---\n";
  obj2.info();

  // ----------------------------------

  return 0;
}
