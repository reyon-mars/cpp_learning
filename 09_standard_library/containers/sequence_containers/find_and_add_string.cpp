#include <forward_list>
#include <iostream>
#include <string>
#include <cstddef>
#include <algorithm>   // 🔹 NEW

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

// Get last element safely (tiny helper)
std::string last_element(const std::forward_list<std::string>& lst)
{
    std::string last;
    for (const auto& s : lst)
        last = s;
    return last;
}

// ---- VERY SMALL EXTRA HELPERS ----

// Count occurrences of a value
std::size_t count_value(const std::forward_list<std::string>& lst,
                        const std::string& value)
{
    std::size_t count = 0;
    for (const auto& s : lst)
        if (s == value)
            ++count;
    return count;
}

// Tiny divider
void print_divider()
{
    std::cout << "----------------------\n";
}

// 🔹 NEW: print first N elements
void print_first_n(const std::forward_list<std::string>& lst, int n)
{
    int count = 0;
    for (const auto& s : lst) {
        if (count++ >= n) break;
        std::cout << s << " ";
    }
    std::cout << "\n";
}

// 🔹 NEW: check if all strings are non-empty
bool all_non_empty(const std::forward_list<std::string>& lst)
{
    return std::all_of(lst.begin(), lst.end(),
        [](const std::string& s) {
            return !s.empty();
        });
}

// 🔹 NEW: total character count
std::size_t total_characters(const std::forward_list<std::string>& lst)
{
    std::size_t total = 0;
    for (const auto& s : lst)
        total += s.size();

    return total;
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
        cout << "Last element: " << last_element(lst) << "\n";
    }

    cout << "Contains 'cherry'? "
         << (contains(lst, "cherry") ? "Yes\n" : "No\n");

    // ---- very tiny extra usage ----
    cout << "Count of 'cherry': "
         << count_value(lst, "cherry") << "\n";

    print_divider();

    // --------------------------------------
    // EXTRA SMALL ADDITIONS
    // --------------------------------------

    // push_front example
    lst.push_front("first");
    cout << "After push_front: ";
    print_list(lst);

    // pop_front example
    lst.pop_front();
    cout << "After pop_front: ";
    print_list(lst);

    // remove a value
    lst.remove("cherry");
    cout << "After removing 'cherry': ";
    print_list(lst);

    // reverse list
    lst.reverse();
    cout << "Reversed list: ";
    print_list(lst);

    // sort list
    lst.sort();
    cout << "Sorted list: ";
    print_list(lst);

    // ---------------- NEW SMALL ADDITIONS ----------------

    // remove_if (remove strings with length < 6)
    lst.remove_if([](const std::string& s) {
        return s.length() < 6;
    });
    cout << "After remove_if (len < 6): ";
    print_list(lst);

    // unique (remove consecutive duplicates)
    lst.push_front("apple");
    lst.push_front("apple");
    lst.unique();
    cout << "After unique(): ";
    print_list(lst);

    // splice_after (move elements from another list)
    std::forward_list<string> extra = {"x", "y"};
    lst.splice_after(lst.before_begin(), extra);
    cout << "After splice_after at beginning: ";
    print_list(lst);

    // check empty again
    cout << "Is list empty now? "
         << (lst.empty() ? "Yes\n" : "No\n");

    // 🔹 NEW TESTS

    // print first 3 elements
    cout << "First 3 elements: ";
    print_first_n(lst, 3);

    // all strings non-empty?
    cout << "All strings non-empty? "
         << (all_non_empty(lst) ? "Yes\n" : "No\n");

    // total characters
    cout << "Total character count: "
         << total_characters(lst) << "\n";

    // emplace_front example
    lst.emplace_front("starter");
    cout << "After emplace_front: ";
    print_list(lst);

    // resize list
    lst.resize(5);
    cout << "After resize(5): ";
    print_list(lst);

    // ----------------------------------------------------

    // Clear list
    lst.clear();
    cout << "After clear, size: " << list_size(lst) << "\n";

    return 0;
}
