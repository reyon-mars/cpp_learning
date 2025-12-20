#include <list>
#include <iostream>

// ---------------- ORIGINAL LOGIC ----------------

void process_original_list() {
    std::list<int> lst = { 1, 2, 3, 4, 5, 6, 7 , 8 , 9 };
    auto iter = lst.begin();

    while (iter != lst.end()) {
        if (*iter % 2) {
            iter = lst.insert(iter, *iter);
            std::advance(iter, 2);
        } else {
            iter = lst.erase(iter);
        }
    }

    std::cout << "Original processed list: ";
    for (auto item : lst) {
        std::cout << item << " ";
    }
    std::cout << "\n";
}

// ---------------- EXTRA UTILITIES ----------------

void printList(const std::list<int>& L) {
    std::cout << "List: ";
    for (auto v : L)
        std::cout << v << " ";
    std::cout << "\n";
}

std::list<int> makeList(int start, int end) {
    std::list<int> L;
    for (int i = start; i <= end; ++i)
        L.push_back(i);
    return L;
}

std::list<int> squareList(const std::list<int>& L) {
    std::list<int> squared;
    for (int v : L)
        squared.push_back(v * v);
    return squared;
}

void printReversed(const std::list<int>& L) {
    std::cout << "Reversed list: ";
    for (auto it = L.rbegin(); it != L.rend(); ++it)
        std::cout << *it << " ";
    std::cout << "\n";
}

// Demonstrates erase/remove idiom
void remove_multiples_of_3(std::list<int>& L) {
    L.remove_if([](int x) { return x % 3 == 0; });
}

// ---------------- MAIN ----------------

int main(void) {

    // Original behavior
    process_original_list();

    std::cout << "\n[Extra] Creating list 10..20\n";
    std::list<int> extraList = makeList(10, 20);
    printList(extraList);

    std::cout << "[Extra] Removing multiples of 3\n";
    remove_multiples_of_3(extraList);
    printList(extraList);

    std::cout << "\n[Extra] Squaring list\n";
    auto squared = squareList(extraList);
    printList(squared);

    std::cout << "[Extra] Printing reversed squared list\n";
    printReversed(squared);

    return 0;
}
