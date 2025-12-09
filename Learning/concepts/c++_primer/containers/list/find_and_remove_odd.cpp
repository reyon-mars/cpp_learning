#include <forward_list>
#include <iostream>

void find_and_remove_odd(  std::forward_list<int> &lst ){
    auto  prev = lst.before_begin();
    auto curr = lst.begin();
    
    while( curr != lst.end() ){
        if( *curr % 2 ){
            curr = lst.erase_after( prev );
        }
        else{
            prev = curr;
            curr++;
        }
    }
}

void print_list( std::forward_list<int>& lst ){
    for( auto elem: lst ){
        std::cout << '[' << elem << ']' << "->";
    }
    std::cout << "\n";
    return ;
}

int main() {
    // Test case 1: normal list with mixed numbers
    std::forward_list<int> fl1 = {1, 2, 3, 4, 5, 6, 7, 8};
    std::cout << "Original list 1: ";
    print_list(fl1);

    find_and_remove_odd(fl1);
    std::cout << "After removing odd numbers: ";
    print_list(fl1);

    // Test case 2: all odd numbers
    std::forward_list<int> fl2 = {1, 3, 5, 7, 9};
    std::cout << "\nOriginal list 2: ";
    print_list(fl2);

    find_and_remove_odd(fl2);
    std::cout << "After removing odd numbers: ";
    print_list(fl2);

    // Test case 3: all even numbers
    std::forward_list<int> fl3 = {2, 4, 6, 8, 10};
    std::cout << "\nOriginal list 3: ";
    print_list(fl3);

    find_and_remove_odd(fl3);
    std::cout << "After removing odd numbers: ";
    print_list(fl3);

    // Test case 4: empty list
    std::forward_list<int> fl4;
    std::cout << "\nOriginal list 4 (empty): ";
    print_list(fl4);

    find_and_remove_odd(fl4);
    std::cout << "After removing odd numbers: ";
    print_list(fl4);

    return 0;
}

// ------------------------------------------------
// EXTRA ADDED CODE BELOW (original code untouched)
// ------------------------------------------------

std::forward_list<int> generate_random_list(int n) {
    std::forward_list<int> lst;
    for (int i = n; i >= 1; --i)
        lst.push_front((i * 7) % 13); // some pattern-based pseudo-random numbers
    return lst;
}

void test_random_list() {
    auto lst = generate_random_list(10);
    std::cout << "\n[Extra] Random list: ";
    print_list(lst);

    find_and_remove_odd(lst);

    std::cout << "[Extra] After removing
