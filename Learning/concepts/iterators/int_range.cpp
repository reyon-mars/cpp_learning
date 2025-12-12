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

void print_range(const int_range& r) {
    std::cout << "Range: ";
    for (int v : r) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
}

int sum_range(const int_range& r) {
    int total = 0;
    for (int v : r) total += v;
    return total;
}

struct RangeTester {
    RangeTester() {
        std::cout << "[Extra] Testing int_ran_
