#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>
#include <cassert>
#include <optional>
#include <numeric>

[[nodiscard]] int calculate() noexcept { return 42; }

struct [[nodiscard]] ImportantResult { int value; };

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

[[nodiscard]] Status get_status() { return {true, "Everything is fine"}; }

enum class OperationState { Success, Failure };

[[nodiscard]] OperationState perform_operation() noexcept {
    return OperationState::Success;
}

class Calculator {
public:
    [[nodiscard]] int add(int a, int b) const noexcept { return a + b; }
    [[nodiscard]] int sub(int a, int b) const noexcept { return a - b; }
    [[nodiscard]] int mul(int a, int b) const noexcept { return a * b; }
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

void use_result(int value) { std::cout << "Using result: " << value << '\n'; }

void print_divider() { std::cout << "----------------------\n"; }

void attribute_summary() {
    std::cout << "\nAttribute Summary:\n"
              << "  [[nodiscard]]       -> warns if result is ignored\n"
              << "  [[deprecated]]      -> warns when the entity is used\n"
              << "  [[maybe_unused]]    -> suppresses unused-variable warnings\n"
              << "  [[likely]]          -> hints branch is likely taken\n"
              << "  [[unlikely]]        -> hints branch is unlikely taken\n"
              << "  [[no_unique_address]]-> allows empty member optimisation\n"
              << "  [[carries_dependency]]-> memory-order dependency propagation\n";
}

template<typename T>
void print_type_info(std::string_view name) {
    std::cout << name
              << " trivially_copyable=" << std::boolalpha << std::is_trivially_copyable_v<T>
              << " standard_layout="    << std::is_standard_layout_v<T> << '\n';
}

[[nodiscard]] int hot_path_value(int x) noexcept {
    if (x > 0) [[likely]]
        return x * 2;
    else [[unlikely]]
        return 0;
}

[[nodiscard]] int safe_divide(int a, int b) noexcept {
    if (b == 0) [[unlikely]]
        return 0;
    return a / b;
}

struct Empty {};

struct WithNoUniqueAddress {
    [[no_unique_address]] Empty   e;
    int                           value = 0;
};

struct WithoutNoUniqueAddress {
    Empty e;
    int   value = 0;
};

[[nodiscard]] std::optional<int>
try_parse(std::string_view sv) noexcept {
    if (sv.empty()) [[unlikely]] return std::nullopt;
    int result = 0;
    for (char c : sv) {
        if (c < '0' || c > '9') [[unlikely]] return std::nullopt;
        result = result * 10 + (c - '0');
    }
    return result;
}

struct [[nodiscard]] ParseResult {
    bool        ok;
    int         value;
    std::string error;
};

[[nodiscard]] ParseResult parse_int(std::string_view sv) noexcept {
    if (sv.empty()) [[unlikely]]
        return {false, 0, "empty input"};
    int result = 0;
    for (char c : sv) {
        if (c < '0' || c > '9') [[unlikely]]
            return {false, 0, "invalid character"};
        result = result * 10 + (c - '0');
    }
    return {true, result, ""};
}

template<typename Alloc = std::allocator<int>>
struct IntBuffer {
    [[no_unique_address]] Alloc alloc;
    std::vector<int>            data;

    explicit IntBuffer(std::initializer_list<int> il) : data{il} {}
    [[nodiscard]] int sum() const noexcept {
        return std::accumulate(data.begin(), data.end(), 0);
    }
};

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
    std::cout << "Sub: " << calc.sub(10, 4) << '\n';
    std::cout << "Mul: " << calc.mul(3, 7)  << '\n';

    NewSystem newSys;
    newSys.run();

    (void)calculate();

    auto status = get_status();
    std::cout << "Status OK:      " << (status.ok ? "Yes" : "No") << '\n';
    std::cout << "Status message: " << status.message              << '\n';

    print_divider();

    const std::vector<int> nums{1, 2, 3};
    for ([[maybe_unused]] int n : nums) {}
    std::cout << "Loop with [[maybe_unused]] completed\n";

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
    print_type_info<WithNoUniqueAddress>("WithNoUniqueAddress");

    print_divider();
    auto state = perform_operation();
    std::cout << "Operation: "
              << (state == OperationState::Success ? "Success" : "Failure") << '\n';

    [[maybe_unused]] constexpr std::array<int, 3> sample{1, 2, 3};

    std::cout << "\n=== [[likely]] / [[unlikely]] ===\n";
    std::cout << "hot_path(5)="  << hot_path_value(5)  << '\n';
    std::cout << "hot_path(-1)=" << hot_path_value(-1) << '\n';
    std::cout << "safe_divide(10,2)=" << safe_divide(10, 2) << '\n';
    std::cout << "safe_divide(10,0)=" << safe_divide(10, 0) << '\n';

    std::cout << "\n=== [[no_unique_address]] ===\n";
    std::cout << "sizeof(WithNoUniqueAddress)="    << sizeof(WithNoUniqueAddress)    << '\n';
    std::cout << "sizeof(WithoutNoUniqueAddress)=" << sizeof(WithoutNoUniqueAddress) << '\n';
    std::cout << "size reduced: "
              << std::boolalpha
              << (sizeof(WithNoUniqueAddress) <= sizeof(WithoutNoUniqueAddress)) << '\n';

    std::cout << "\n=== try_parse (optional) ===\n";
    for (std::string_view sv : {"123", "45a", "", "999"}) {
        if (auto v = try_parse(sv))
            std::cout << '"' << sv << "\" -> " << *v << '\n';
        else
            std::cout << '"' << sv << "\" -> nullopt\n";
    }

    std::cout << "\n=== ParseResult (nodiscard struct) ===\n";
    for (std::string_view sv : {"256", "abc", ""}) {
        auto [ok, value, error] = parse_int(sv);
        if (ok)
            std::cout << '"' << sv << "\" ok=" << ok << " value=" << value << '\n';
        else
            std::cout << '"' << sv << "\" error=" << error << '\n';
    }

    std::cout << "\n=== IntBuffer (no_unique_address alloc) ===\n";
    IntBuffer buf{1, 2, 3, 4, 5};
    std::cout << "sum=" << buf.sum() << '\n';

    assert(calculate()           == 42);
    assert(compute_value()       == 99);
    assert(hot_path_value(3)     == 6);
    assert(hot_path_value(-1)    == 0);
    assert(safe_divide(9, 3)     == 3);
    assert(safe_divide(9, 0)     == 0);
    assert(try_parse("42").value_or(-1) == 42);
    assert(!try_parse("abc").has_value());
    assert(buf.sum()             == 15);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
