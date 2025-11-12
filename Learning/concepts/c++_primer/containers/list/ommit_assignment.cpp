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