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

// ---------------- SMALL EXTRA CODE (ADDED ONLY) ----------------

// check if value exists in range
bool contains(const int_range& r, int value) {
    for (int v : r)
        if (v == value)
            return true;
    return false;
}

// count elements in range
int range_size(const int_range& r) {
    int count = 0;
    for (int _ : r) {
        (void)_;
        ++count;
    }
    return count;
}

// sum of range
int range_sum(const int_range& r) {
    int sum = 0;
    for (int v : r)
        sum += v;
    return sum;
}

// check if range is empty
bool empty(const int_range& r) {
    return range_size(r) == 0;
}

// reusable printer helper
void print_range(const int_range& r) {
    for (int v : r)
        std::cout << v << " ";
    std::cout << "\n";
}

// ---------------- MAIN ----------------

int main(void) {
    int_range r(1, 6);

    std::cout << "Range values: ";
    print_range(r);

    // ---- small added usage ----
    std::cout << "Contains 3? "
              << (contains(r, 3) ? "Yes" : "No") << "\n";

    std::cout << "Range size: " << range_size(r) << "\n";
    std::cout << "Range sum: " << range_sum(r) << "\n";
    std::cout << "Is range empty? "
              << (empty(r) ? "Yes" : "No") << "\n";
    // --------------------------

    return 0;
}
