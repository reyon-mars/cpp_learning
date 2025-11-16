#include <vector>
#include <numeric>
#include <utility>

auto sum_vector( std::vector<int> vec ){
  return [ v = std::move( vec ) ]()mutable{
    int sum = std::accumulate( v.begin(), v.end(), 0 );
    v.clear();
    return sum;
  };
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    auto fn = sum_vector(vec);
    int s1 = fn();   // returns 15
    int s2 = fn();   // returns 0 (vector already cleared)

    return 0;
}
