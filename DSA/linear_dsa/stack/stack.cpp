#include <iostream>

class Stack {
  private:
    int stack[ 100 ];
    int StackPointer;
  public:
    Stack( void ){
      StackPointer = 0;
      }
    void push( int element ){
      if( StackPointer == 100 ){
        std:: cout << "Stack Overflow" << std:: endl;
        return ;
      }
      stack[ StackPointer++ ] = element;
      return ;
    }

    int pop( void ){
      if( StackPointer == 0 ){
        std:: cout << "Stack Empty" << std:: endl;
        return -1;
      }
      return stack[ -- StackPointer ];
    }
};

class AddingStack: Stack {
  private:
    int sum;
  public:
    AddingStack( void ): Stack() {
      sum = 0;
      }
    void push(int element ){
      sum += element;
      Stack:: push( element );
    }
    int pop( void ){
      int element = Stack::pop( );
      sum -= element;
      return element;
    }
    int getSum( void ){
      return sum;
    }
};

int main(void) {
  AddingStack super_stack;

  for(int i = 1; i < 10; i++)
    super_stack.push(i);
  std:: cout << super_stack.getSum() << std::endl;
  for(int i = 1; i < 10; i++)
    super_stack.pop();
  std::cout << super_stack.getSum() << std::endl;
  return 0;
}