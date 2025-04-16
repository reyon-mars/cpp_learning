#include <iostream>

int abs( int n ){
    return ( n < 0 ) ? ( n * -1 ) : n;
}

int main(){
    int n(0), m(0);

    while( true ){
        std::cout << "Please enter a number n : ";
        std::cin >> n;
        std::cout << "Please enter a number m : ";
        std::cin >> m;

        n = abs(n);
        m = abs(m);
        
        if( m == 0 ){ return 0; }

        std::cout << n << " " << m << std::endl;

        int lowerClosestNumber = n - ( n % m );
        int upperClosestNumber = n + ( m - ( n % m ));

        std::cout << "Lower Closest " << lowerClosestNumber << std::endl;
        std::cout << "Upper Closest " << upperClosestNumber << std::endl;

        int closestNumber = ( ( n - lowerClosestNumber ) == ( upperClosestNumber - n ) ) ?
                            ( upperClosestNumber ) : ( ( n - lowerClosestNumber ) > ( upperClosestNumber - n ) ? 
                            upperClosestNumber : lowerClosestNumber );
        

        std::cout << "The closest number to " << n <<  " divisible by " << m << " is " << closestNumber << std::endl;

    }

}