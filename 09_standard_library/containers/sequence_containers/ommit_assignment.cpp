#include <iostream>
#include <list>
#include <numeric>     // tiny addition
#include <algorithm>   // tiny addition
#include <iterator>    // NEW small addition

// -------------------------------------------------------
// SMALL ADDITION (FORWARD DECLARATION ONLY)
// -------------------------------------------------------
void print_list(const std::list<int>& lst);

// ---- very tiny extra forward declarations ----
int sum_list(const std::list<int>& lst);
int max_list_safe(const std::list<int>& lst);
void print_divider();

// ---- additional tiny helpers ----
int min_list_safe(const std::list<int>& lst);
double average_list(const std::list<int>& lst);

// ---- extra tiny helpers ----
bool contains_value(const std::list<int>& lst, int value);
int count_value(const std::list<int>& lst, int value);

// ---- NEW small helpers ----
void print_front_back(const std::list<int>& lst);
bool all_positive(const std::list<int>& lst);

// ---------------- ORIGINAL CODE (UNCHANGED) ----------------

int main(void)
{
    std::list<int> v = {1, 2, 3};
    auto begin = v.begin();

    while (begin != v.end())
    {
        ++begin;             // move to next
        v.insert(begin, 42); // insert before it
        ++begin;             // move past the element
    }

    for (int n : v)
        std::cout << n << " ";

    std::cout << std::endl;

    // -------- SMALL ADDITIONS --------
    print_list(v);
    std::cout << "List size: " << v.size() << std::endl;
    std::cout << (v.empty() ? "List is empty\n"
                            : "List is not empty\n");

    // ---- very tiny extra usage ----
    std::cout << "Sum: " << sum_list(v) << std::endl;
    std::cout << "Max: " << max_list_safe(v) << std::endl;

    // ---- additional tiny usage ----
    std::cout << "Min: " << min_list_safe(v) << std::endl;
    std::cout << "Average: " << average_list(v) << std::endl;

    // ---- extra tiny usage ----
    std::cout << "Contains 42? "
              << (contains_value(v, 42) ? "Yes" : "No") << std::endl;

    std::cout << "Count of 42: "
              << count_value(v, 42) << std::endl;

    print_divider();

    // ---- extra small operations ----

    // reverse list
    v.reverse();
    std::cout << "Reversed list: ";
    print_list(v);

    // sort list
    v.sort();
    std::cout << "Sorted list: ";
    print_list(v);

    // remove value
    v.remove(42);
    std::cout << "After removing 42: ";
    print_list(v);

    // unique (remove consecutive duplicates)
    v.push_back(1);
    v.push_back(1);
    v.unique();
    std::cout << "After unique(): ";
    print_list(v);

    // ---------------- NEW SMALL ADDITIONS ----------------

    // push_front and push_back
    v.push_front(100);
    v.push_back(200);

    std::cout << "After push_front/back: ";
    print_list(v);

    // front and back values
    print_front_back(v);

    // pop operations
    v.pop_front();
    v.pop_back();

    std::cout << "After pop_front/back: ";
    print_list(v);

    // check positivity
    std::cout << "All positive? "
              << (all_positive(v) ? "Yes" : "No") << std::endl;

    // resize list
    v.resize(6, 9);
    std::cout << "After resize: ";
    print_list(v);

    // clear list
    std::list<int> temp = v;
    temp.clear();
    std::cout << "Temp list cleared. Size: "
              << temp.size() << std::endl;

    // -----------------------------------------------------

    return 0;
}

// -------------------------------------------------------
// SMALL EXTRA CODE (HELPER ONLY)
// -------------------------------------------------------

void print_list(const std::list<int>& lst)
{
    std::cout << "[Extra] List contents: ";
    for (int x : lst)
        std::cout << x << " ";
    std::cout << std::endl;
}

// ---- VERY SMALL EXTRA HELPERS ----

// Sum elements
int sum_list(const std::list<int>& lst)
{
    return std::accumulate(lst.begin(), lst.end(), 0);
}

// Safe max finder
int max_list_safe(const std::list<int>& lst)
{
    if (lst.empty()) return 0;
    return *std::max_element(lst.begin(), lst.end());
}

// ---- NEW SMALL HELPERS ----

// Safe min finder
int min_list_safe(const std::list<int>& lst)
{
    if (lst.empty()) return 0;
    return *std::min_element(lst.begin(), lst.end());
}

// Average calculation
double average_list(const std::list<int>& lst)
{
    if (lst.empty()) return 0.0;
    return static_cast<double>(sum_list(lst)) / lst.size();
}

// Check if value exists
bool contains_value(const std::list<int>& lst, int value)
{
    for (int v : lst)
        if (v == value)
            return true;
    return false;
}

// Count occurrences of a value
int count_value(const std::list<int>& lst, int value)
{
    int count = 0;
    for (int v : lst)
        if (v == value)
            ++count;
    return count;
}

// Print front and back safely
void print_front_back(const std::list<int>& lst)
{
    if (!lst.empty()) {
        std::cout << "Front: " << lst.front() << std::endl;
        std::cout << "Back: " << lst.back() << std::endl;
    }
}

// Check if all elements are positive
bool all_positive(const std::list<int>& lst)
{
    for (int v : lst)
        if (v <= 0)
            return false;
    return true;
}

// Tiny divider
void print_divider()
{
    std::cout << "----------------------" << std::endl;
}
