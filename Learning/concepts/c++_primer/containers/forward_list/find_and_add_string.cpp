#include <forward_list>
#include <iostream>
#include <string>

// ======================================================
// ORIGINAL CODE (LOGIC PRESERVED)
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

// ======================================================
// SMALL EXTRA HELPERS (ADDITION ONLY)
// ======================================================

// Count elements in the list
std::size_t list_size(const std::forward_list<std::string>& lst)
{
    std::size_t count = 0;
    for (const auto& _ : lst) {
        (void)_;
        ++count;
    }
    return count;
}

// Check if a value exists
bool contains(const std::forward_list<std::string>& lst,
              const std::string& value)
{
    for (const auto& s : lst)
        if (s == value)
            return true;
    return false;
}

// ======================================================
// MAIN
// ======================================================

int main() {
    using namespace std;

    forward_list<string> lst = {"apple", "banana", "cherry", "date"};

    cout << "Original list:\n";
    print_list(lst);
    cout << "Size: " << list_size(lst) << "\n\n";

    // Replace in the middle
    find_and_add(lst, "banana", "blueberry");
    cout << "After replacing 'banana':\n";
    print_list(lst);
    cout << "Size: " << list_size(lst) << "\n\n";

    // Replace first element
    find_and_add(lst, "apple", "apricot");
    cout << "After replacing 'apple':\n";
    print_list(lst);
    cout << "Size: " << list_size(lst) << "\n\n";

    // Replace last element
    find_and_add(lst, "date", "dragonfruit");
    cout << "After replacing 'date':\n";
    print_list(lst);
    cout << "Size: " << list_size(lst) << "\n\n";

    // Insert when not found
    find_and_add(lst, "kiwi", "kumquat");
    cout << "After inserting 'kumquat':\n";
    print_list(lst);
    cout << "Size: " << list_size(lst) << "\n";

    // ---- small additions ----
    cout << "List is "
         << (lst.empty() ? "empty\n" : "not empty\n");

    if (!lst.empty()) {
        cout << "First element: " << lst.front() << "\n";
    }

    cout << "Contains 'cherry'? "
         << (contains(lst, "cherry") ? "Yes\n" : "No\n");

    // Clear list
    lst.clear();
    cout << "After clear, size: " << list_size(lst) << "\n";
    // -------------------------

    return 0;
}
