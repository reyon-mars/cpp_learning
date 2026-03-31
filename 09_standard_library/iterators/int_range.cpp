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

// find min value
int range_min(const int_range& r) {
    int min = *r.begin();
    for (int v : r)
        if (v < min)
            min = v;
    return min;
}

// find max value
int range_max(const int_range& r) {
    int max = *r.begin();
    for (int v : r)
        if (v > max)
            max = v;
    return max;
}

// print reversed view (no modification)
void print_reverse(const int_range& r) {
    int last = 0;
    for (int v : r) last = v;
    for (int i = last; contains(r, i); --i)
        std::cout << i << " ";
    std::cout << "\n";
}

// ---- ADDITIONAL TINY HELPERS ----

// check if range is empty
bool is_empty(const int_range& r) {
    return r.begin() == r.end();
}

// get first element
int first_element(const int_range& r) {
    return *r.begin();
}

// get last element
int last_element(const int_range& r) {
    int last = 0;
    for (int v : r)
        last = v;
    return last;
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

    std::cout << "Range min: " << range_min(r) << "\n";
    std::cout << "Range max: " << range_max(r) << "\n";

    std::cout << "Reversed view: ";
    print_reverse(r);

    // ---- tiny extra usage ----
    std::cout << "Is empty? "
              << (is_empty(r) ? "Yes" : "No") << "\n";

    std::cout << "First element: " << first_element(r) << "\n";
    std::cout << "Last element: " << last_element(r) << "\n";

    return 0;
}
