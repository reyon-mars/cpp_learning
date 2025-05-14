#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>
template< class T >
std::vector<std::vector<T>>  transpose_matrix( const std::vector<std::vector<T>>& matrix ){
    
    if( matrix.empty() || matrix[0].empty() ){ return {}; }

    size_t rows = matrix.size(), cols = matrix[0].size();
    for( const auto& row: matrix ){
        if( row.size() != cols ){
            throw std::invalid_argument("Jagged matrix.");
        }
    }

    if( rows == cols ){

        std::vector<std::vector<T>> transpose( matrix );
        
        for( int i = 0; i < rows-1; i++ ){
            for( int j = i+1; j < cols; j++ ){
                std::swap( transpose[i][j], transpose[j][i] );
            }   
        }
        return transpose;
        
    } else{

        std::vector<std::vector<T>> transpose( cols, std::vector<T> (rows) );
        for( int i = 0; i < rows; i++ ){
            for( int j = 0; j < cols; j++ ){
                transpose[j][i] = matrix[i][j];
            }
        }
        return transpose;

    }
}