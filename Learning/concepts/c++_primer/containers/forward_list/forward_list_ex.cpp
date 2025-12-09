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

void print_forward_list(const std::forward_list<int>& fl) {
    std::cout << "\n[Extra] List contents: ";
    for (auto e : fl) std::cout << e << " ";
    std::cout << std::endl;
}

std::forward_list<int> generate_list(int n) {
    std::forward_list<int> lst;
    for (int i = n; i >= 1; --i)
        lst.push_front(i); // push_front builds in reverse order
    return lst;
}

// Auto-run extra code before main() executes
int _ = [](){
    std::forward_list<int> lst = generate_list(10);
    print_forward_list(lst);
    return 0;
}();
