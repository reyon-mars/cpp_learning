#include <iostream>
typedef char string[100];


void strcpy( const char *str, char *dest ){
    int i = 0;    
    for( ; str[i] != '\0' && i < 100; i++ ){
        dest[i] = str[i];
    }
    dest[i] = '\0';
}


int sterlen( const char* str ){
    int size(0);
    for(; str[size] != '\0'; size++ );
    return size;
}

char* reverse_string( const char* input_str ){
    int size(0);
    while( *(input_str+size) != '\0' ){
        size++;
    }
    char* result = new char[size + 1];
    int start(0), end(size-1);
    while( start < size ){
        result[start] = input_str[end];
        start++;
        end--;
    }
    result[size] = '\0';
    return result;
}

int main(){

    string str;
    
    std::cout << "Please enter a string: ";
    std::cin >> str;
    
    std::cout << sterlen( str );


    char* copy = new char[100];
    strcpy( str, copy );
    std::cout << copy << std::endl;
    delete []copy;

    char* rev = reverse_string( str );
    std::cout << rev << std::endl;
    delete[] rev;
    
    return 0;
    
}