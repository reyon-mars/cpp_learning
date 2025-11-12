#include <list>
#include <iostream>

int main( void ){
    std::list<int> lst = { 1, 2, 3, 4, 5, 6, 7 , 8 , 9 };
    auto iter = lst.begin();    

    while( iter != lst.end() ){
        if( *iter % 2 ){
            iter = lst.insert( iter, *iter );
            std::advance( iter, 2 );
        }else{
            iter = lst.erase( iter );
        }
    }

    for( auto item: lst ){
        std::cout << item << " "; 
    }
    std::cout << std::endl;
    return 0;
}