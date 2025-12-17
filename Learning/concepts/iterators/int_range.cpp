class int_range{
    private:
    int start;
    int end;

    public:
    
    class Iterator {
        private:
        int current_;

        public:
        explicit Iterator( int current ) : current_( current ){};
        
        int operator*()const{
            return current_;
        }
        
        Iterator& operator++( ){
            ++current_;
            return *this;
        }

        bool operator!=( const Iterator& other ){
            return current_ != other.current_;
        }
    };

    explicit int_range( int s, int e ) : start(s), end(e) {};
    
    Iterator begin(){
        return Iterator( start );
    }
    Iterator end() {
        return Iterator( end );
    }
};


// ======================================================
// EXTRA CODE ADDED BELOW (original class unchanged)
// ======================================================

#include <iostream>

// Print all values in the range
void print_range(const int_range& r) {
    std::cout << "Range: ";
    for (int v : r) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
}

// Sum all values in the range
int sum_range(const int_range& r) {
    int total = 0;
    for (int v : r)
        total += v;
    return total;
}

// Count number of elements in the range
int count_range(const int_range& r) {
    int count = 0;
    for (int v : r) {
        (void)v;
        ++count;
    }
    return count;
}

// Runs automatically before main()
struct RangeTester {
    RangeTester() {
        std::cout << "[Extra] Testing int_range\n";

        int_range r1(1, 6);
        print_range(r1);

        std::cout << "Sum: " << sum_range(r1) << '\n';
        std::cout << "Count: " << count_range(r1) << '\n';

        int_range r2(10, 15);
        print_range(r2);

        std::cout << "Sum: " << sum_range(r2) << '\n';
        std::cout << "Count: " << count_range(r2) << '\n';
    }
};

// Global object → executes before main()
RangeTester __range_test;
