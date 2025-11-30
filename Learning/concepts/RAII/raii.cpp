#include <iostream>
#include <fstream>

class file_guard{
    private:
    std::fstream f_handle;

    public:
    explicit file_guard ( const std::string& filename ) : f_handle( filename, std::ios::in ) 
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