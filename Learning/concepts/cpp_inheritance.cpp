#include <iostream>
using namespace std;
// Definition of a Super Class or Base Class
class Super {
  private:
    int storage; // private attribute
  protected:
    int accessible_storage; // protected attribute copy of storage
  public:
    Super(){ storage = 0; accessible_storage = 0; } // constructor  to initialize the storage and accessible_storage value
    void set( int val ) { storage = accessible_storage = val; } // setter method
    int get() { return storage; } // getter method
};
// Definition of another Base class
class Base {
  protected:
    int storage;
  public:
    Base() { storage = 0; };
    void set( int val ) { storage = val; }
    int get() { return storage; }
};
// Sub Class inheriting from the Super class as public inheritance and single inheritance
class Sub : public Super {
  public:
    void  print( void ) { cout << " Storage = " << accessible_storage << endl; } // accessible_storage can be used because it is protected
};
// Sub Class inheriting from multiple class Super and Base with public visibility
class SubMulti: public Super, public Base {
  public:
    void print( void ) {
      cout << " Storage Super = " << Super:: accessible_storage << endl;
      cout << " Storage Base = " << Base :: storage << endl;
      return ;
      }
};


int main( void ) {
  SubMulti object;
  object.Super::set( 100 );
  object.Super::set( object.Super::get() + 1 );
  object.print();

  return 0;
}