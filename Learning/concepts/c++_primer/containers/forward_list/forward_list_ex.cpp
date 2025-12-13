#include <forward_list>
#include <iostream>


int main ( void ){
    
    std::forward_list<int> f_lst = { 0,1,2,3,4,5,6,7,8,9 };
    auto prev = f_lst.before_begin();
    auto curr = f_lst.begin();
    
    while( curr != f_lst.end() ){
        if( *curr % 2  ){
            curr = f_lst.erase_after( prev );
        } else{
            prev = curr;
            curr++;
        }
    }

    for( auto elem: f_lst ){
        std::cout <<  elem << " ";
    }

    return 0;
}

// ----------------------------------------------------
// EXTRA CODE ADDED BELOW (original code untouched)
// ----------------------------------------------------

void show_even_numbers(const std::forward_list<int>& fl) {
    std::cout << "\n[Extra] Even numbers: ";
    for (auto v : fl)
        if (v % 2 == 0) std::cout << v << " ";
    std::cout << std::endl;
}

// Multiply all elements by a given factor and return a new list
std::forward_list<int> multiply_list(const std::forward_list<int>& fl, int factor) {
    std::forward_list<int> result;
    auto it = result.before_begin();

    for (int v : fl) {
        it = result.insert_after(it, v * factor);
    }
    return result;
}

// Automatically executed before main()
int __extra = [](){
    std::forward_list<int> test = {2, 4, 6};

    show_even_numbers(test);

    auto multiplied = multiply_list(test, 3);
    std::cout << "[Extra] Multiplied list: ";
    for (int v : multiplied)
        std::cout << v << " ";
    std::cout << std::endl;

    return 0;
}();
