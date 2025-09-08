#include<iostream>
#include<cmath>
#include<math.h>

class string{
    public:
    
    char intToAscii( short number )
    {
        return (char) ( '0' + number );
    }

    void push( char* string, char character )
    {
        int strlen = 0;
        while ( string[ strlen ] != '\0' ){
            strlen++;
        }
        int stringEnd = strlen;
        string[ stringEnd + 1 ] = '\0';
        string[ stringEnd ] = character; 
    }

    void pushFront( char* string , char character )
    {
        int strlen = 0; // string length counter
        while ( string[ strlen ] != '\0' ) // run the loop until the nul terminator 
        {
            strlen++; // increase the length on each iteration 
        }
        int stringEnd = strlen, currentPosition = strlen; // index of the nul terminator 
        string[ stringEnd + 1 ] = '\0'; // shift the nul terminator to one index higher
        while( strlen-- ){
            string[ currentPosition ] = string[ currentPosition - 1]; // shift each character in the string towards right
            currentPosition--; // decrease the current position of shifting 
        }
        string[ 0 ] = character; // add the new character to the start of the string 
        return ;
    }
};

class math{
    public:

    bool checkPrime( int number )
    {
        bool flag = 1;
        for( int i = 2; i <= sqrt( number ); i++ )
        {
            if( number % i == 0 ){
                std::cout << number << " is not a prime number.";
                std::cout << std::endl ;
                flag = 0;
                break;
            }
        }
        if( flag == 1)
        {
            std::cout << number << " is a prime number";
            std::cout << std::endl;
        }
        return flag;
    }

    int radixReverse( int number )
    {
        int reverse = 0;
        while( number > 0 )
        {
            int lastDigit = number % 10;
            reverse = reverse * 10 + lastDigit;
            number = number / 10;
        }
        return reverse;
    }

    bool isArmstrongNumber( int number )
    {
        int originalNumber = number;
        int sum = 0;
        while( number > 0)
        {
            int lastDigit = number % 10;
            sum += pow( lastDigit, 3 );
            number /= 10;
        }
        if( sum == originalNumber )
        {
            std:: cout << originalNumber << " is an Armstrong number";
            std:: cout << std:: endl;
            return true;
        }
        else
        {
            std:: cout << originalNumber << " is not an Armstrong number";
            std:: cout << std:: endl;
            return false;
        }
    }

    unsigned long int factorialRecursion( unsigned long long int number )
    {
        if( number > 1 ){
            return number * factorialRecursion( number - 1 );
        }
        else
        {
            return 1;
        }

    }

    bool isPrime( int number )
    {
        for( int i = 2; i <= sqrt( number ); i++){
            if( number % i == 0 ){
                return false;
            }
        }
        return true;
    }

    void primeInBetween( int a, int b)
    {
        if( a < b ){
            for( int i = a; i <= b; i++){
                if( isPrime(i) ){
                    std:: cout << i << std:: endl;
                }
            }
        }
        else {
            for( int i = b; i <= a; i++ ){
                if( isPrime( i )){
                    std:: cout << i << std:: endl;
                }
            }
        }
        return;
    }
    
    void fibonacci( int n )
    {
        int term1 = 0;
        int term2 = 1;
        int nextTerm;

        for( int i = 1; i <= n; i++ ){
            std:: cout << term1 << std:: endl;
            nextTerm = term1 + term2;
            term1 = term2;
            term2 = nextTerm;
        }
        return;
    }

    int factorial( int n )
    {
        int product = 1;
        for( int i = 2; i <= n; i++){
            product *= i;
        }
        return product;
    }

    int binomialCoeff( int n, int r )
    {
        return factorial( n ) / ( factorial( n-r ) * factorial( r ) );
    }

    void pascalTriangle( int n )
    {
        for( int i = 0; i <= n; i++){
            for( int j = 0; j <= i; j++){
                std:: cout << binomialCoeff( i, j ) << " ";
            }
            std:: cout << std:: endl;
        }
    }

    int sumNatural( int n )
    {
        if( n >= 1 ){
            return n + sumNatural( n - 1);
        }
        else{
            return 0;
        }
    }

    int exponent( int a, int b )
    {
        int value = 1;
        while( b-- ){
            value *= a;
        }
        return abs(value);
    }

    int square( int n )
    {
        return exponent( n, 2 );
    }
    
    int max( int a, int b )
    {
        return (( a > b) * a) + (( b > a ) * b) + ( a == b) * a;
    }

    bool pythagorasTriplet( int x, int y, int z )
    {
        int hypotenuse = max( x, max( y, z ));
        if( x == hypotenuse){
            return square( x ) == ( square( y ) + square( z ) );
        }
        else if( y == hypotenuse ){
            return square( y ) == ( square( x ) + square( z ) );
        }
        else{
            return square( z ) == ( square( x ) + square( y ) );
        }
    }

    int decimalToBinary1( int number )
    {
        int binaryValue = 0;
        int lastBinaryDigit = 0;
        while( number > 0 ){
            lastBinaryDigit = number % 2;
            binaryValue = binaryValue * 10 + lastBinaryDigit;
            number /= 2;
        }
        return binaryValue;
    }

    int convertToDecimal( int number, int radixBase )
    {
        int decimalValue = 0;
        int i = 0;
        while( number > 0 ){
            decimalValue += ( number % 10 ) * exponent( radixBase, i );
            number /= 10;
            i++;
        }
        return decimalValue;
    }
    int binaryToDecimal( int number )
    {
        int decimalValue = 0;
        int i = 0;
        while( number > 0 ){
            decimalValue += ( number % 10 ) * exponent( 2, i );
            number /= 10;
            i++;
        }
        return decimalValue;
    }

    int octalToDecimal( int number )
    {
        int decimalValue = 0;
        int i = 0;
        while( number > 0 ){
            decimalValue += ( number % 10 ) * exponent( 8 , i );
            number /= 10;
            i++;
        }
        return decimalValue;
    }

    int hexadecimalToDecimal( char* hexNumber )
    {
        int decimalValue = 0;
        int i = 0;
        int strlen = 0;
        while( hexNumber[ strlen ] != '\0' ){
            bool lowercase = ( hexNumber[ strlen ] >= 'a' && hexNumber[ strlen ] <= 'f');
            if( lowercase ){
                hexNumber[ strlen ] -= 32;
            }
            strlen++;
        };

        while( hexNumber[ i ] != '\0' ){
            --strlen;
            if( hexNumber[ strlen ] >= '0' &&  hexNumber[ strlen ] <= '9' )
            {
                decimalValue += ( hexNumber[ strlen ] - '0' ) * exponent( 16, i );
            }
            else if( hexNumber[ strlen ] >= 'A' && hexNumber[ strlen ] <= 'F') 
            {
                decimalValue += ( hexNumber[ strlen ] - 'A' + 10 ) * exponent( 16, i );
            }
            i++;
        }
        return decimalValue;
    }

    int decimalToBinary( int number )
    {
        int binaryExponent = 1;
        int binaryValue = 0;
        while( binaryExponent <= number ){
            binaryExponent *= 2;
        }
        binaryExponent /= 2;

        while( binaryExponent > 0 ){
            int lastBinaryDigit = number / binaryExponent;
            number -= lastBinaryDigit * binaryExponent;
            binaryValue = ( binaryValue * 10 ) + lastBinaryDigit;
            binaryExponent /= 2;
        }
        return binaryValue;
    }

    int decimalToOctal( int number )
    {
        int octalExponent = 1;
        int octalValue = 0;
        while( octalExponent <= number ){
            octalExponent *= 8;
        }
        octalExponent /= 8;

        while ( octalExponent > 0 ){
            int lastOctalDigit = number / octalExponent;
            number -= lastOctalDigit * octalExponent;
            octalValue = ( octalValue * 10 ) + lastOctalDigit;
            octalExponent /= 8;
        }
        return octalValue;
    }

    char* decimalToHexadecimal( int number )
    {
        string str;
        int hexadecimalExponent = 1;
        static char hexadecimalValue[ 50 ];
        memset( hexadecimalValue, 0, sizeof( hexadecimalValue ) );
        if( number == 0 ){
            str.push( hexadecimalValue, str.intToAscii( !( number == 0 ) ) );
        }
        while( hexadecimalExponent <= number ){
            hexadecimalExponent *= 16;
        }
        hexadecimalExponent /= 16;

        while ( hexadecimalExponent > 0 ){
            int lastHexadecimalDigit = number / hexadecimalExponent;
            number -= lastHexadecimalDigit * hexadecimalExponent;
            hexadecimalExponent /= 16;
            if( lastHexadecimalDigit <= 9 ){
                str.push( hexadecimalValue, str.intToAscii( lastHexadecimalDigit ) );
            }
            else{
                char hexadecimalChar = 'A' + ( lastHexadecimalDigit - 10 );
                str.push( hexadecimalValue, hexadecimalChar );
            }
        }
        
        return hexadecimalValue;
    }


    int reverseInt( int number )
    {
        int reversedInt = 0;
        while( number > 0 ){
            int firstDigit = number % 10;
            reversedInt = reversedInt * 10 + firstDigit;
            number /= 10;
        }
        return reversedInt;
    }

    int binaryAddition( int A, int B )
    {
        int binarySum = 0;
        int initialZeroes = 0;
        bool carryBit = false;

        while( A > 0 && B > 0 ){
            if( A % 2 == 0 && B % 2 == 0 ){
                binarySum = binarySum * 10 + (int)carryBit;
                carryBit = false;
            }
            else if( ( A % 2 == 0 && B % 2 == 1 ) || ( A % 2 == 1 && B % 2 == 0 ) ){
                if( carryBit ){
                    binarySum = binarySum * 10 + 0;
                    carryBit = true;
                }
                else{
                    binarySum = binarySum * 10 + 1;
                    carryBit = false;
                }
            }
            else{
                binarySum = binarySum * 10 + (int)carryBit;
                carryBit = true;
            }
            initialZeroes += 1 * ( binarySum == 0 );
            A /= 10;
            B /= 10;
        }
        
        while( A > 0 ){
            if( carryBit == true ){
                if( A % 2 == 1 ){
                    binarySum = binarySum * 10 + 0;
                    carryBit = true;
                }
                else{
                    binarySum = binarySum * 10 + 1;
                    carryBit = false;
                }
            }
            else{
                binarySum = binarySum * 10 + ( A % 2 );
            }
            A /= 10;
        }
        while( B > 0 ){
            if( carryBit == true ){
                if( B % 2 == 1 ){
                    binarySum = binarySum * 10 + 0;
                    carryBit = true;
                }
                else{
                    binarySum = binarySum * 10 + 1;
                    carryBit = false;
                }
            }
            else{
                binarySum = binarySum * 10 + ( B % 2 );
            }
            B /= 10;
        }
        if( carryBit ){
            binarySum = binarySum * 10 + 1;
        }

        binarySum = reverseInt( binarySum ) * exponent( 10 , initialZeroes );
        return binarySum;
    }

    float max( float a, float b, float c )
    {
        return a > b ? ( a > c ? a : c ) : ( b > c ? b : c );
    }

    int *MinMaxInArray( int *array, int length) {
      int max = array[0];
      int min = array[0];
      for( int i = 1; i < length; i++ ){
        if( array[i] > max || array[i] < min ){
          ( array[i] > max ) ? max = array[i] : min = array[i];
        }
      }
      std:: cout << min << std:: endl;
      static int MinMax[2] = { MinMax[0] = min ,  MinMax[1] = max };
      return MinMax;
    }

    int linearSearch( int *array, int length , int value ) {
      for( int i = 0; i < length; i++ ){
        if( array[i] == value ){
          return i;
        }
      }
      return -1;
    }

    int* bubblesort( int* array , size_t arrayLen )
    {
        int auxillary;
        bool swapped;
        do{
            swapped = false;
            for( size_t i = 0; i < arrayLen - 1; i++ ){
                if( array[ i ] > array[ i + 1 ])
                {
                    auxillary = array[ i + 1 ];
                    array[ i + 1 ] = array[ i ];
                    array[ i ] = auxillary;
                    swapped = true;
                }
            }
        } while( swapped );
        return array;
    }

    int* selectionSort( int* array, size_t length )
    {
      int min = 0;

      for( size_t i = 0; i < length-1; i++ ){

        int index_min = i;
        int index_initial = i;
        min = array[ index_min ];

        for( size_t j = i + 1; j < length; j++ ){
          	if( array[ j ] <= min ){
            	min = array[ j ];
            	index_min = j;
        	}
        }

        int auxilary = array[ index_initial ];
        array[ index_initial ] = array[ index_min ];
        array[ index_min ] = auxilary;
      }
      return array;
    }

};

int main( void )
{   
    math maths;
	int array[] = { 90, 32, 53, 12, 35, 13, 152, 24, 14, 18, 11 };
    int* minMax = maths.MinMaxInArray( array, sizeof( array ) / sizeof( int ) );
    std:: cout << minMax[0] << " " << minMax[1] << std:: endl;
    std:: cout << maths.binaryAddition( 110 , 101 ) << std:: endl;
    int indexOfKey;
    std:: cout << ( indexOfKey = maths.linearSearch( array, sizeof( array ) / sizeof( int ) , 18 ) )<< std::endl;
    std:: cout << array[ indexOfKey ] << std:: endl;
    maths.selectionSort( array, sizeof( array ) / sizeof( int ) );
	std:: cout << "sorted array " << std:: endl;
    for( int i = 0; i < array[ 0 ]; i++ ){
      std:: cout << array[ i ] << " ";
    }

    return 0;
}