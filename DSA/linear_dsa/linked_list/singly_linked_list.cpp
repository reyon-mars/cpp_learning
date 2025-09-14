#include <iostream>

class list{
  public:
    list() : head( nullptr ) {};
    

  private:
    struct Node{
        int data;
        Node* next;
        Node( int data_ ) : data( data_ ), next( nullptr ){};
    };

    Node* head;
}
