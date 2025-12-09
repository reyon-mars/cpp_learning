#include <iostream>
#include <list>

int main(void)
{
    std::list<int> v = {1, 2, 3};
    auto begin = v.begin();

    while (begin != v.end())
    {
        ++begin;             // move to next
        v.insert(begin, 42); // insert before it, but ignore returned iterator
        ++begin;             // move past the element that was originally after insert
    }

    for (int n : v)
        std::cout << n << " ";

    return 0;
    
}

// -------------------------------------------------------
// EXTRA CODE BELOW (original code left completely intact)
// -------------------------------------------------------

void print_list(const std::list<int>& lst) {
    std::cout << "\n[Extra] List: ";
    for (auto x : lst) std::cout << x << " ";
    std::cout << "\n";
}

std::list<int> make_pattern_list(int size) {
    std::list<int> lst;
    for (int i = 1; i <= size; ++i)
        lst.push_back(i * i);  // squares
    return lst;
}

int __ = [](){
    std::cout << "\n=== EXTRA AUTOMATIC TEST ===\n";
    auto lst = make_pattern_list(7);
    print_list(lst);

    lst.remove_if([](int x){ return x % 2 == 0; });
    std::cout << "[Extra] After removing even squares: ";
    print_list(lst);

    return 0;
}();
