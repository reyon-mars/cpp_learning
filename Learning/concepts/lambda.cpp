#include <vector>
#include <numeric>
#include <utility>

auto sum_vector( std::vector<int> vec ){
  return [ v = std::move( vec ) ](){
    int sum = std::accumulate( v.begin(), v.end(), 0 );
    v.clear();
    return sum;
  }
}


int main ( void ){
  std::vector<int> vec = { 1, 2, 3, 4, 5 };

}
