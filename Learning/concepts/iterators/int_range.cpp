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