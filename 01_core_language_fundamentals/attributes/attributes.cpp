#include <iostream>
#include <utility>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <type_traits>
#include <span>
#include <ranges>

[[nodiscard]] int calculate() noexcept { return 42; }

struct [[nodiscard]] ImportantResult {
    int value;
};

[[nodiscard("You must use the return value")]]
int compute_value() noexcept { return 99; }

[[deprecated("Use new_function instead")]]
void old_function() { std::cout << "Old function\n"; }

void new_function() { std::cout << "New function\n"; }

void debug_log([[maybe_unused]] int level) noexcept {}

[[maybe_unused]] void helper() { std::cout << "Helper\n"; }

struct [[nodiscard]] Status {
    bool        ok;
    std::string message;
};

[[nodiscard]] Status get_status() {
    return {true, "Everything is fine"};
}

enum class OperationState { Success, Failure };

[[nodiscard]] OperationState perform_operation() noexcept {
    return OperationState::Success;
}

class Calculator {
public:
    [[nodiscard]] int add(int a, int b) const noexcept { return a + b; }
};

class [[deprecated("Use NewSystem instead")]] OldSystem {
public:
    void run() { std::cout << "Running old system\n"; }
};

class NewSystem {
public:
    void run() { std::cout << "Running new system\n"; }
};

[[deprecated("Use new_version instead")]] [[maybe_unused]]
inline constexpr int old_version = 1;

[[maybe_unused]] constexpr auto quick_lambda = []() noexcept {
    std::cout << "Quick lambda\n";
};

void use_result(int value) {
    std::cout << "Using result: " << value << '\n';
}

void print_divider() { std::cout << "----------------------\n"; }

void attribute_summary() {
    std::cout << "\nAttribute Summary:\n"
              << "[[nodiscard]]     -> warns if result is ignored\n"
              << "[[deprecated]]    -> warns when the entity is used\n"
              << "[[maybe_unused]]  -> suppresses unused-variable warnings\n";
}

template<typename T>
void print_type_info(std::string_view name) {
    std::cout << name
              << " is trivially copyable? "
              << (std::is_trivially_copyable_v<T> ? "Yes" : "No")
              << '\n';
}

int main() {
    [[maybe_unused]] auto result = calculate();
    use_result(calculate());

    ImportantResult res{100};
    std::cout << "ImportantResult: " << res.value << '\n';

    new_function();
    debug_log(1);

    [[maybe_unused]] constexpr int temp = 10;

    [[maybe_unused]] auto val = compute_value();
    std::cout << "Computed value: " << val << '\n';

    Calculator calc;
    [[maybe_unused]] auto sum = calc.add(5, 3);
    std::cout << "Sum: " << sum << '\n';

    NewSystem newSys;
    newSys.run();

    (void)calculate();

    auto status = get_status();
    std::cout << "Status OK:      " << (status.ok ? "Yes" : "No") << '\n';
    std::cout << "Status message: " << status.message << '\n';

    print_divider();

    const std::vector<int> nums = {1, 2, 3};
    for ([[maybe_unused]] int n : nums) {}
    std::cout << "Loop with [[maybe_unused]] variable completed\n";

    print_divider();

    quick_lambda();

    print_divider();

    auto [first, second] = std::make_pair(10, 20);
    [[maybe_unused]] int ignored = second;
    std::cout << "First value: " << first << '\n';

    print_divider();

    attribute_summary();

    print_divider();

    print_type_info<ImportantResult>("ImportantResult");
    print_type_info<Status>("Status");

    print_divider();

    auto state = perform_operation();
    std::cout << "Operation state: "
              << (state == OperationState::Success ? "Success" : "Failure")
              << '\n';

    [[maybe_unused]] constexpr std::array<int, 3> sample = {1, 2, 3};

    return 0;
}
