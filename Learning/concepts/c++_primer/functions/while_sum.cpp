#include <iostream>

int natural_sum( int n )
{
	int sum = 0;
	while( n )
	{
		sum += n;
		n--;
	}
	return sum;
}

int main ( void ){
    int sum = 0;
    int num = 50;
    while( num <= 100 ){
        sum += num;
        num++;
    }
    std::cout << sum  << std::endl;
    return 0;
}
