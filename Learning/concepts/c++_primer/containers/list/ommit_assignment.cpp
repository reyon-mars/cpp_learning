#include <iostream>
#include <list>

// -------------------------------------------------------
// SMALL ADDITION (FORWARD DECLARATION ONLY)
// -------------------------------------------------------
void print_list(const std::list<int>& lst);

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
    // -------------------------------

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
