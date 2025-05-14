#include <string>
#include <vector>

int longest_substr_with_k_distinct( const std::string& str , int k ){
    std::vector<int> char_freq( 128, 0 );
    int left(0), right(0), unique_char(0), max_len(0);
    
    while( right < str.size() ){

        char curr_char = str[right++];

        if( ++char_freq[curr_char] == 1 ){
            unique_char++;
        }

        while( unique_char > k ){
            char curr_char = str[left++];
            if( --char_freq[curr_char] == 0 ){
                unique_char--;
            }
        }

        max_len = std::max( max_len, right - left );
    }
    return max_len;
}