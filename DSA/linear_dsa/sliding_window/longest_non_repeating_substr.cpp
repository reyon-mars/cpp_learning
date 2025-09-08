#include <string>
#include <vector>

int longest_non_repeating_substr( const std::string& str ){
    
    std::vector<int> seen_at( 128, -1 );
    int left(0), right(0), max_len(0);
    
    while( right < str.size() ){
        char curr_char = str[right];
        
        if( seen_at[curr_char] >= 0 ){
            left = std::max( left, seen_at[curr_char] + 1);
        }
        seen_at[curr_char] = right;
        max_len = std::max( max_len, right - left + 1 );
        right++;
    }
    return max_len;
}