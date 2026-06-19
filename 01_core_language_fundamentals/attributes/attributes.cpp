// Attributes Exercise
// [[nodiscard]], [[deprecated]], [[maybe_unused]]

#include <iostream>
#include <utility>   // for structured binding pair
#include <string>
#include <vector>
#include <type_traits>
#include <array>

[[nodiscard]] int calculate() {
    return 42;
}

// Apply nodiscard to a type
struct [[nodiscard]] ImportantResult {
    int value;
};

[[deprecated("Use new_function instead")]]
void old_function() {
    std::cout << "Old function\n";
}

void new_function() {
    std::cout << "New function\n";
}

// maybe_unused parameter 
void debug_log([[maybe_unused]] int level) {
    // intentionally unused
}

// maybe_unused function
[[maybe_unused]] void helper() {
    std::cout << "Helper function\n";
}

// nodiscard with message (C++20)
[[nodiscard("You must use the return value")]]
int compute_value() {
    return 99;
}

// nodiscard method inside class
class Calculator {
public:
    [[nodiscard]] int add(int a, int b) const {
        return a + b;
    }
};

// deprecated class
class [[deprecated("Use NewSystem instead")]] OldSystem {
public:
    void run() {
        std::cout << "Running old system\n";
    }
};

class NewSystem {
public:
    void run() {
        std::cout << "Running new system\n";
    }
};

void use_result(int value) {
    std::cout << "Using result: " << value << "\n";
}

// nodiscard with custom type
struct [[nodiscard]] Status {
    bool ok;
    std::string message;
};

Status get_status() {
    return {true, "Everything is fine"};
}

// maybe_unused lambda
[[maybe_unused]] auto quick_lambda = []() {
    std::cout << "Quick lambda executed\n";
};

// deprecated variable 
[[maybe_unused]] [[deprecated("Use new_version instead")]]
int old_version = 1;

// helper to print divider
void print_divider() {
    std::cout << "----------------------\n";
}

// compile-time attribute info
void attribute_summary() {
    std::cout << "\nAttribute Summary:\n";
    std::cout << "[[nodiscard]] -> warns if result ignored\n";
    std::cout << "[[deprecated]] -> warns when entity is used\n";
    std::cout << "[[maybe_unused]] -> suppresses unused warnings\n";
}

// simple template utility
template<typename T>
void print_type_info(const std::string& name) {
    std::cout << name
              << " is trivially copyable? "
              << (std::is_trivially_copyable<T>::value ? "Yes" : "No")
              << "\n";
}

// nodiscard enum-style result
enum class OperationState {
    Success,
    Failure
};

[[nodiscard]]
OperationState perform_operation() {
    return OperationState::Success;
}

int main() {

    // nodiscard used correctly
    [[maybe_unused]] auto result = calculate();
    use_result(calculate());


    // Using nodiscard type
    ImportantResult res{100};
    std::cout << "ImportantResult: " << res.value << "\n";

    // deprecated function (kept for demonstration)
    // old_function();

    new_function();
    debug_log(1);

    [[maybe_unused]] int temp = 10;

    [[maybe_unused]] int val = compute_value();
    std::cout << "Computed value: " << val << "\n";

    Calculator calc;
    [[maybe_unused]] int sum = calc.add(5, 3);

    std::cout << "Sum: " << sum << "\n";


    NewSystem newSys;
    newSys.run();

    std::pair<int, int> p = {1, 2};

    (void)calculate(); // intentional ignore (no warning in some compilers)

    Status status = get_status();

    std::cout << "Status OK: "
              << (status.ok ? "Yes" : "No") << "\n";

    std::cout << "Status message: "
              << status.message << "\n";

    print_divider();

    std::vector<int> nums = {1, 2, 3};

    for ([[maybe_unused]] int n : nums) {
        // intentionally unused loop variable
    }

    std::cout << "Loop with maybe_unused variable completed\n";

    print_divider();

    quick_lambda();


    print_divider();

    auto [first, second] = std::make_pair(10, 20);

    [[maybe_unused]] int ignored = second;

    std::cout << "First value: " << first << "\n";

    print_divider();

    attribute_summary();

    print_divider();

    print_type_info<ImportantResult>("ImportantResult");
    print_type_info<Status>("Status");

    print_divider();

    auto state = perform_operation();

    std::cout << "Operation state: "
              << (state == OperationState::Success
                  ? "Success"
                  : "Failure")
              << "\n";

    [[maybe_unused]] std::array<int, 3> sample = {1, 2, 3};

    return 0;
}
