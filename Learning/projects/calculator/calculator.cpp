#include <iostream>
#include <vector>

int power( int a , int b ){
    if( b < 0 ){
        return -1;
    }
    if( b == 0 ){
        return 1;
    }
    if( b == 1 ){
        return a;
    }
    return power( a, b-1 ) * a;
}


int main(){

    char rerun('y');

    while( rerun == 'y' || rerun == 'Y' ){

        int num1(0);
        std::cout << "Please enter a number: ";
        std::cin >> num1;
        
        int num2(0);
        std::cout << "Please enter the second number: ";
        std::cin >> num2;
        
        char op(' ');
        std::cout << "Please select the operation to perform: ";
        std::cin >> op;

        switch( op ){
            case '+':
                std::cout << num1 + num2 << "\n";
                break;

            case '-':
                std::cout << num1 - num2 << "\n";
                break;

            case '*':
                std::cout << num1 * num2 << "\n";
                break;

            case '/':
                if( num2 == 0 ){
                    std::cout << "Division by zero is undefined." << std::endl;
                    break;
                }
                std::cout <<  num1 / (float) num2  << "\n";
                break;

            case '%':
                std::cout << num1 % num2 << "\n";
                break;

            case '^':
                std::cout << power( num1 , num2 ) << "\n";
                break;
            
            default:
                std::cout << "Invalid operator. " << "\n";
        }
        std::cout << "Do you wish to exit ? ( y/n ) " << "\n";
        std::cin >> rerun;
}
}