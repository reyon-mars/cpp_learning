#include <iostream>

using namespace std;

class Patterns
{
public:
    void solidRectangle(int row, int col)
    {
        for (int i = 1; i <= row; i++)
        {
            for (int j = 1; j <= col; j++)
            {
                cout << "*";
            }
            cout << endl;
        }
    }

    void hollowRectangle(int row, int col)
    {
        for (int i = 1; i <= row; i++)
        {
            for (int j = 1; j <= col; j++)
            {
                if (i == 1 || i == row)
                {
                    cout << "*";
                }
                else if (j == 1 || j == col)
                {
                    cout << "*";
                }
                else
                {
                    cout << " ";
                }
            }
            cout << endl;
        }
    }

    void invertedTriangle(int row)
    {
        int base = row;
        for (int i = 1; i <= row; i++)
        {
            for (int j = base; j >= 1; j--)
            {
                cout << "*";
            }
            base--;
            cout << endl;
        }
    }

    void rightTriangle(int row)
    {
        for (int i = 1; i <= row; i++)
        {
            for (int j = 1; j <= row; j++)
            {
                if (j <= row - i)
                {
                    cout << " ";
                }
                else
                {
                    cout << "*";
                }
            }
            cout << endl;
        }
    }
    
    void numberPyramid( int row )
    {
        for( int i = 1; i <= row; i++){
            for( int j = 1; j<=i; j++){
                cout << i;
            }
            cout << endl;
        }
    }

    void folydsTriangel( int row )
    {
        int number = 1;
        for( int i = 1; i <= row; i++){
            for( int j = 1; j <= i; j++){
                cout<<number;
                number++;
            }
            cout << endl;
        }
    }

    void butterfly( int n)
    {
        for( int i = 1; i <= n; i++){
            for( int j = 1; j <= i; j++){
                cout << "*";
            }
            int space = 2*n - 2*i;
            for( int j = 1; j <= space; j++){
                cout << " ";
            }
            for( int j = 1; j <= i; j++){
                cout << "*";
            }
            cout << endl;
        }
        for( int i = n; i >= 1; i--){
            for( int j = 1; j <= i; j++){
                cout << "*";
            }
            int space = 2*n - 2*i;
            for( int j = 1; j <= space; j++){
                cout << " ";
            }
            for( int j =1; j <= i; j++){
                cout << "*";
            }
            cout << endl;
        }        
    }

    void invertedNumberPyramid( int base )
    {
        for( int i = 1; i <= base; i++){
            int digit = 1;
            for( int j = 1; j <= (base + 1)- i; j++ ){
                cout << digit;
                digit++;
            }
            cout << endl; 
        }
    }

    void binaryTriangle( int base )
    {
        for( int i = 1; i <= base; i++){
            for( int j = 1; j <= i; j++ ){
                if( (i + j) % 2 == 0 )
                {
                    cout << 1 << " ";
                }
                else
                {
                     cout << 0 << " ";
                }
            }
            cout << endl;
        }
    }

    void rhombus( int base )
    {
        for( int i = 1; i <= base; i++){
            for( int j = 1; j <= base - i; j++){
                cout << " ";
            }
            for( int k = 1; k <= base; k++){
                cout << " * ";
            }
            cout << endl;
        }
    }

    void numberTriangle( int base )
    {
        for( int i = 1; i <= base; i++ ){
            for( int j = 1; j <= base - i; j++){
                cout << " ";
            }

            for( int k = 1; k <= i; k++){
                cout << k << " ";
            }
            cout << endl;
        }
    }
    
    void palindromicTriangle( int base )
    {
        for( int i = 1; i <= base; i++){
            for( int j = 1; j <= base-i; j++){
                cout << "  ";
            }
            for( int k = i; k >= 1; k--){
                cout << k << " ";
            }
            int ascending = 2;
            for( int l = 1; l <= i - 1; l++){
                cout << ascending++ << " ";
            }
            cout << endl;
        }
    }
    void palindromicTriangle2( int base )
    {
        for( int i = 1; i <= base; i++){
            int j;
            for( j = 1; j <= base-i; j++){
                cout << "  ";
            }
            int k = i;
            for( ; j <= base; j++){
                cout << k-- << " ";
            }
            k = 2;
            for( ; j <= base + i - 1; j++){
                cout << k++ << " ";
            }
            cout << endl; 
        }
    }

    void diamond( int base )
    {
        int n = 0;
        for( int i = 1; i <= base; i++){
            for( int j = 1; j <= base - i; j++){
                cout << "  ";
            }
            for( int k = 1; k <= (2*n)+1; k++ ){
                cout << "*" << " ";
            }
            n++;
            cout << endl;
        }

        int m = base - 1;
        for( int i = base; i >= 1; i--){
            for( int j = 1; j <= base-i; j++){
                cout << "  ";
            }
            for( int k = 1; k <= (2*m)+1; k++ ){
                cout << "*" << " ";
            }
            m--;
            cout << endl;
        }
    }

    void diamond2( int base ){
        for( int i = 1; i <= base; i++){
            for( int j = 1; j <= base - i; j++){
                cout << "  ";
            }
            for( int k = 1; k <= (2*i)-1; k++){
                cout << "*" << " ";
            }
            cout << endl;
        }
        for( int i = base; i >= 1; i--){
            for( int j = 1; j <= base-i; j++){
                cout << "  ";
            }
            for( int k = 1; k <= (2*i)-1; k++){
                cout << "*" << " ";
            }
            cout << endl;
        }
    }

    void zigzag( int n ){
        for ( int i = 1; i <= 3; i++){
            for( int j = 1; j <= n; j++){
                if((( i+j )%4 == 0) || ( i == 2 && j%4 == 0)){
                    cout << "*" << " ";
                }
                else {
                    cout << "  ";
                }
            }
            cout << endl;
        }
    }
};

int main(void)
{
    int row;
    cout << "Please enter a number: ";
    cin >> row ;

    Patterns patterns;
    patterns.zigzag( row );

    return 0;
}
