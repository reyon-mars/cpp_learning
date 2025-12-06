#include <iostream>

int main( void ){
    std::cout << "Hello, World" << std::endl;
    return 0;
    
}

void greet(){
    std::cout << "This is extra code added without modifying main!" << std::endl;
}

int add(int a, int b){
    return a + b;
}

class Demo{
public:
    void show(){
        std::cout << "Demo class method executed!" << std::endl;
    }
};
