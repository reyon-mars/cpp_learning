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

        int operator*() const { return current_; }

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

// ---------------- SMALL ADDITIONS ----------------

// check if value exists
bool contains(const int_range& r, int value) {
    for (int v : r)
        if (v == value)
            return true;
    return false;
}

// count elements
int range_size(const int_range& r) {
    int count = 0;
    for (int _ : r) {
        (void)_;
        ++count;
    }
    return count;
}

// sum of elements
int range_sum(const int_range& r) {
    int sum = 0;
    for (int v : r)
        sum += v;
    return sum;
}

// ---- VERY SMALL EXTRA ----

// average of range
double range_average(const int_range& r) {
    int sz = range_size(r);
    return sz ? static_cast<double>(range_sum(r)) / sz : 0.0;
}

// print reversed view (no modification)
void print_reverse(const int_range& r) {
    int last = 0;
    for (int v : r) last = v;
    for (int i = last; contains(r, i); --i)
        std::cout << i << " ";
    std::cout << "\n";
}

// ---------------- MAIN ----------------

int main(void) {
    int_range r(1, 6);

    std::cout << "Range values: ";
    for (int v : r)
        std::cout << v << " ";
    std::cout << "\n";

    std::cout << "Contains 3? "
              << (contains(r, 3) ? "Yes" : "No") << "\n";

    std::cout << "Range size: " << range_size(r) << "\n";
    std::cout << "Range sum: " << range_sum(r) << "\n";
    std::cout << "Range average: " << range_average(r) << "\n";

    std::cout << "Reversed view: ";
    print_reverse(r);

    return 0;
}
