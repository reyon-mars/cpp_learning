#include <iostream>

int main( void ){
    int N(0);
    std::cin >> N;
    
    for( u_int32_t i = 1; i <= N; i++ ){
        for( u_int32_t j = 2; j < i; j++ ){
            if( i % j == 0 ){
                continue;
            }
        }
        std::cout << "Prime Number " << i << std::endl;
    }
}