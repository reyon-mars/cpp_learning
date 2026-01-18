#include <iostream>

// ---------------- ORIGINAL CODE (UNCHANGED) ----------------

class int_range {
private:
    int start;
    int end;

public:
    class Iterator {
    private:
        int current_;

    public:
        explicit Iterator(int current) : current_(current) {}

        int operator*() const {
            return current_;
        }

        Iterator& operator++() {
            ++current_;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current_ != other.current_;
        }
    };

    explicit int_range(int s, int e) : start(s), end(e) {}

    Iterator begin() { return Iterator(start); }
    Iterator end()   { return Iterator(end); }

    Iterator begin() const { return Iterator(start); }
    Iterator end()   const { return Iterator(end); }
};

// ---------------- SMALL EXTRA CODE ----------------

// Check if a value exists in the range
bool contains(const int_range& r, int value) {
    for (int v : r) {
        if (v == value)
            return true;
    }
    return false;
}

// Small helper: count elements in range
int range_size(const int_range& r) {
    int count = 0;
    for (int _ : r) {
        (void)_;
        ++count;
    }
    return count;
}

// ---- very small added helper ----
// Sum of all elements in the range
int range_sum(const int_range& r) {
    int sum = 0;
    for (int v : r)
        sum += v;
    return sum;
}
// ---------------------------------

// ---------------- MAIN ----------------

int main() {
    std::cout << "--- int_range Demo ---\n";

    int_range r(1, 6);

    std::cout << "Range: ";
    for (int v : r)
        std::cout << v << ' ';
    std::cout << '\n';

    std::cout << "Contains 3? "
              << (contains(r, 3) ? "Yes" : "No") << '\n';

    std::cout << "Contains 10? "
              << (contains(r, 10) ? "Yes" : "No") << '\n';

    // ---- small added usage ----
    std::cout << "Range size: " << range_size(r) << '\n';
    std::cout << "Range sum: " << range_sum(r) << '\n';
    // --------------------------

    return 0;
}
