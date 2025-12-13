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

// --------------------------------------------------------
// MORE EXTRA CODE ADDED BELOW (original + extra untouched)
// --------------------------------------------------------

std::list<int> squareList(const std::list<int>& L) {
    std::list<int> squared;
    for (int v : L) squared.push_back(v * v);
    return squared;
}

void printReversed(const std::list<int>& L) {
    std::cout << "[Extra] Reversed list: ";
    for (auto it = L.rbegin(); it != L.rend(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";
}

// Runs before main()
int ___ = [](){
    std::list<int> demo = makeList(1, 5);

    auto squared = squareList(demo);
    printList(squared);

    printReversed(squared);
    return 0;
}();
