#include <forward_list>
#include <iostream>
#include <string>

void find_and_add( std::forward_list<std::string>& lst, std::string&s1, std::string& s2 ){
    auto prev = lst.before_begin();
    auto curr = lst.begin();
    
    for( auto str: lst ){
        if( str == s1 ){
            lst.erase_after( prev );
            lst.insert_after( prev, s2 );
            return;
        }
        prev = curr;
        curr++;
    }
    lst.insert_after( prev, s2 );
    return;    
}


void print_list( std::forward_list<std::string> lst ){
    for( auto str : lst ){
        std::cout << str << " " ;
    }
    std::cout << std::endl;
}



int main(){
    using namespace std;
    forward_list<string> lst = {"apple", "banana", "cherry", "date"};

    cout << "Original list:\n";
    print_list(lst);
    cout << "\n";

    // ✅ Case 1: Replace an element in the middle ("banana" -> "blueberry")
    {
        string s1 = "banana", s2 = "blueberry";
        find_and_add(lst, s1, s2);
        cout << "After replacing 'banana' with 'blueberry':\n";
        print_list(lst);
        cout << "\n";
    }

    // ✅ Case 2: Replace the first element ("apple" -> "apricot")
    {
        string s1 = "apple", s2 = "apricot";
        find_and_add(lst, s1, s2);
        cout << "After replacing 'apple' with 'apricot':\n";
        print_list(lst);
        cout << "\n";
    }

    // ✅ Case 3: Replace the last element ("date" -> "dragonfruit")
    {
        string s1 = "date", s2 = "dragonfruit";
        find_and_add(lst, s1, s2);
        cout << "After replacing 'date' with 'dragonfruit':\n";
        print_list(lst);
        cout << "\n";
    }

    // ✅ Case 4: Add a new element (not found case)
    {
        string s1 = "kiwi", s2 = "kumquat";
        find_and_add(lst, s1, s2);
        cout << "After inserting 'kumquat' (since 'kiwi' not found):\n";
        print_list(lst);
        cout << "\n";
    }

    return 0;
}