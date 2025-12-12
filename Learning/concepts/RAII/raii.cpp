#include <iostream>
#include <fstream>

class file_guard{
    private:
    std::fstream f_handle;

    public:
    explicit file_guard ( const std::string& filename ) 
        : f_handle( filename, std::ios::in ) 
    {
        if( !( f_handle.is_open() ) ){
            throw std::runtime_error( "Failed to open " + filename );
        }   
    }

    ~file_guard(){
        if( f_handle.is_open() ){
            f_handle.close();
        }
    }

    file_guard( const file_guard& ) = delete;
    file_guard& operator=( const file_guard&  ) = delete;

    std::fstream& get(){ return f_handle; }
};

int main ( void ) {
    file_guard fg_;
    return 0;
}

// ======================================================
// EXTRA CODE ADDED BELOW (original code untouched)
// ======================================================

void test_file_guard(const std::string& name) {
    try {
        file_guard fg(name);
        std::cout << "[Extra] Successfully opened: " << name << "\n";

        std::string line;
        auto& f = fg.get();

        if (std::getline(f, line)) {
            std::cout << "[Extra] First line: " << line << "\n";
        } else {
            std::cout << "[Extra] (File empty)\n";
        }
    }
    catch (const std::exception& ex) {
        std::cout << "[Extra] Error: " << ex.what() << "\n";
    }
}

struct AutoFileTest {
    AutoFileTest() {
        std::cout << "[Extra] Running file_guard test...\n";
        test_file_guard("example.txt");
    }
};

AutoFileTest __auto_file_test;
