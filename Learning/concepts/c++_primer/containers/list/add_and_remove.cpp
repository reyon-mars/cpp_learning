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

// --------------------------------------------------------
// EXTRA CODE ADDED BELOW (original code remains unchanged)
// --------------------------------------------------------

void printList(const std::list<int>& L) {
    std::cout << "[Extra] List: ";
    for (auto v : L) std::cout << v << " ";
    std::cout << "\n";
}

std::list<int> makeList(int start, int end) {
    std::list<int> L;
    for (int i = start; i <= end; ++i)
        L.push_back(i);
    return L;
}

int __ = [](){
    std::list<int> extraList = makeList(10, 20);
    printList(extraList);

    // Modify the list a bit
    extraList.remove_if([](int x){ return x % 3 == 0; });
    std::cout << "[Extra] After removing multiples of 3: ";
    printList(extraList);

    return 0;
}();
