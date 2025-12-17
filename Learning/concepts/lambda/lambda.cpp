#include <vector>
#include <numeric>
#include <utility>

auto sum_vector( std::vector<int> vec ){
    return [ v = std::move( vec ) ]() mutable {
        int sum = std::accumulate( v.begin(), v.end(), 0 );
        v.clear();
        return sum;
    };
}

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    // IILE (Immediately Invoked Lambda Expression)
    int x = [ vec ] ( int a, int b ){
        return std::accumulate( vec.begin(), vec.end(), 0 ) * a * b;
    }( 1, 1 );

    auto fn = sum_vector(vec);
    int s1 = fn();   // returns 15
    int s2 = fn();   // returns 0 (vector already cleared)

    return 0;
}


// ======================================================
// EXTRA CODE ADDED BELOW (original code untouched)
// ======================================================

#include <iostream>

auto make_multiplier(int m) {
    return [m](int x) {
        return x * m;
    };
}

auto capture_by_reference_demo() {
    int value = 10;
    return [&value]() mutable {
        value += 5;
        return value;
    };
}

auto capture_by_value_demo() {
    int value = 20;
    return [value]() mutable {
        value += 10;
        return value;
    };
}

int sum_range(int a, int b) {
    int s = 0;
    for (int i = a; i <= b; ++i)
        s += i;
    return s;
}

struct LambdaTester {
    LambdaTester() {
        std::cout << "[Extra] Testing extra lambdas...\n";

        auto times3 = make_multiplier(3);
        std::cout << "[Extra] 3 * 4 = " << times3(4) << "\n";

        auto ref_lambda = capture_by_reference_demo();
        std::cout << "[Extra] Ref lambda call 1: " << ref_lambda() << "\n";
        std::cout << "[Extra] Ref lambda call 2: " << ref_lambda() << "\n";

        auto val_lambda = capture_by_value_demo();
        std::cout << "[Extra] Val lambda call 1: " << val_lambda() << "\n";
        std::cout << "[Extra] Val lambda call 2: " << val_lambda() << "\n";

        auto sum_lambda = [](int a, int b) {
            return sum_range(a, b);
        };
        std::cout << "[Extra] Sum 1..10 = " << sum_lambda(1, 10) << "\n";
    }
};

// Automatically runs before main()
LambdaTester __auto_lambda_test;
