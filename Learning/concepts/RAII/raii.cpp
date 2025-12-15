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


// ======================================================
// MORE EXTRA CODE (still no changes above)
// ======================================================

// Read entire file contents safely
std::string read_all(file_guard& fg) {
    std::string content, line;
    auto& f = fg.get();

    while (std::getline(f, line)) {
        content += line + '\n';
    }
    return content;
}

// File existence check
bool file_exists(const std::string& name) {
    std::ifstream f(name);
    return f.good();
}

// Auto-run demo for utilities
int __extra2 = [](){
    std::cout << "\n[More Extra]\n";

    const std::string fname = "example.txt";

    if (!file_exists(fname)) {
        std::cout << "File does not exist: " << fname << "\n";
        return 0;
    }

    try {
        file_guard fg(fname);
        std::string data = read_all(fg);
        std::cout << "File contents:\n" << data;
    }
    catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}();


// ======================================================
// EVEN MORE EXTRA CODE (RAII demonstration)
// ======================================================

struct ScopeLogger {
    std::string name;

    explicit ScopeLogger(std::string n) : name(std::move(n)) {
        std::cout << "[Scope] Enter " << name << "\n";
    }

    ~ScopeLogger() {
        std::cout << "[Scope] Exit " << name << "\n";
    }
};

int __extra3 = [](){
    ScopeLogger log("FileGuard Demo Scope");
    std::cout << "[Scope] Doing work...\n";
    return 0;
}();
