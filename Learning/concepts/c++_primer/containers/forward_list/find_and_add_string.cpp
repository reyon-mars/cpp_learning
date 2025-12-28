#include <forward_list>
#include <iostream>
#include <string>

// ======================================================
// ORIGINAL CODE (LOGIC PRESERVED, ITERATION FIXED)
// ======================================================

void find_and_add(std::forward_list<std::string>& lst,
                  const std::string& s1,
                  const std::string& s2)
{
    auto prev = lst.before_begin();
    auto curr = lst.begin();

    while (curr != lst.end()) {
        if (*curr == s1) {
            lst.erase_after(prev);
            lst.insert_after(prev, s2);
            return;
        }
        prev = curr;
        ++curr;
    }

    // if not found → insert at end
    lst.insert_after(prev, s2);
}

void print_list(const std::forward_list<std::string>& lst) {
    for (const auto& str : lst) {
        std::cout << str << " ";
    }
    std::cout << std::endl;
}

int main() {
    using namespace std;

    forward_list<string> lst = {"apple", "banana", "cherry", "date"};

    cout << "Original list:\n";
    print_list(lst);
    cout << "\n";

    // Case 1: Replace an element in the middle
    {
        string s1 = "banana", s2 = "blueberry";
        find_and_add(lst, s1, s2);
        cout << "After replacing 'banana' with 'blueberry':\n";
        print_list(lst);
        cout << "\n";
    }

    // Case 2: Replace the first element
    {
        string s1 = "apple", s2 = "apricot";
        find_and_add(lst, s1, s2);
        cout << "After replacing 'apple' with 'apricot':\n";
        print_list(lst);
        cout << "\n";
    }

    // Case 3: Replace the last element
    {
        string s1 = "date", s2 = "dragonfruit";
        find_and_add(lst, s1, s2);
        cout << "After replacing 'date' with 'dragonfruit':\n";
        print_list(lst);
        cout << "\n";
    }

    // Case 4: Add a new element (not found)
    {
        string s1 = "kiwi", s2 = "kumquat";
        find_and_add(lst, s1, s2);
        cout << "After inserting 'kumquat' (since 'kiwi' not found):\n";
        print_list(lst);
        cout << "\n";
    }

    return 0;
}

// ======================================================
// EXTRA CODE ADDED BELOW (append only)
// ======================================================

bool contains(const std::forward_list<std::string>& lst,
              const std::string& value)
{
    for (const auto& s : lst)
        if (s == value)
            return true;
    return false;
}

std::size_t list_size(const std::forward_list<std::string>& lst) {
    std::size_t count = 0;
    for (const auto& _ : lst) {
        (void)_;
        ++count;
    }
    return count;
}

// Runs before main()
struct ForwardListExtraTests {
    ForwardListExtraTests() {
        std::cout << "\n=== Extra forward_list tests (before main) ===\n";

        std::forward_list<std::string> test = {"one", "two", "three"};

        std::cout << "Initial test list: ";
        print_list(test);

        std::cout << "Contains 'two'? "
                  << (contains(test, "two") ? "yes" : "no") << '\n';

        std::cout << "Contains 'four'? "
                  << (contains(test, "four") ? "yes" : "no") << '\n';

        std::cout << "List size: " << list_size(test) << '\n';

        find_and_add(test, "two", "TWO");

        std::cout << "After replacing 'two' with 'TWO': ";
        print_list(test);
    }
};

ForwardListExtraTests __forward_list_extra_tests;

