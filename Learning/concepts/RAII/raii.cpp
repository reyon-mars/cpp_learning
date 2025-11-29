#include <iostream>
#include <fstream>

class file_guard{
    private:
    std::fstream f_handle;

    public:
    file_guard( std::string filename ){
        f_handle = std::fstream.open( filename.cstr() , std::input );
    }
    ~file_guard(){
        f_handle.close();
    }
}