#include <iostream>
#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <format>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <cassert>
#include <optional>
#include <span>
#include <cstdlib>

enum class StatusCode : int {
    Success       = 0,
    FileError     = 1,
    NetworkError  = 2,
    MemoryError   = 3,
};

[[nodiscard]] constexpr std::string_view to_string(StatusCode c) noexcept {
    switch (c) {
        case StatusCode::Success:      return "SUCCESS";
        case StatusCode::FileError:    return "FILE_ERROR";
        case StatusCode::NetworkError: return "NETWORK_ERROR";
        case StatusCode::MemoryError:  return "MEMORY_ERROR";
    }
    return "UNKNOWN_ERROR";
}

[[nodiscard]] constexpr std::string_view severity(StatusCode c) noexcept {
    switch (c) {
        case StatusCode::Success:      return "NONE";
        case StatusCode::FileError:    return "LOW";
        case StatusCode::NetworkError: return "MEDIUM";
        case StatusCode::MemoryError:  return "HIGH";
    }
    return "UNKNOWN";
}

[[nodiscard]] constexpr bool is_recoverable(StatusCode c) noexcept {
    return c == StatusCode::NetworkError;
}

void print_error(StatusCode c) {
    switch (c) {
        case StatusCode::Success:      std::cout << "No error.\n";             return;
        case StatusCode::FileError:    std::cout << "File error occurred.\n";   return;
        case StatusCode::NetworkError: std::cout << "Network error occurred.\n";return;
        case StatusCode::MemoryError:  std::cout << "Memory error occurred.\n"; return;
    }
    std::cout << "Unknown error.\n";
}

[[nodiscard]] StatusCode perform_task() noexcept {
    return StatusCode::FileError;
}

void log_error(StatusCode c) {
    const auto now = std::chrono::system_clock::now();
    const auto ts  = std::chrono::duration_cast<std::chrono::seconds>(
                         now.time_since_epoch()).count();
    std::cout << std::format("[LOG {}] {}\n", ts, to_string(c));
}

[[nodiscard]] StatusCode retry_task(int attempts) {
    StatusCode result = StatusCode::FileError;
    for (int i : std::views::iota(1, attempts + 1)) {
        std::cout << std::format("Attempt {}...\n", i);
        result = perform_task();
        if (result == StatusCode::Success) return StatusCode::Success;
    }
    return result;
}

class ErrorTracker {
    std::vector<StatusCode> history_;

public:
    void add(StatusCode c) { history_.push_back(c); }

    [[nodiscard]] int total()         const noexcept { return static_cast<int>(history_.size()); }
    [[nodiscard]] std::optional<StatusCode> last() const noexcept {
        if (history_.empty()) return std::nullopt;
        return history_.back();
    }

    [[nodiscard]] int success_count() const {
        return static_cast<int>(std::ranges::count(history_, StatusCode::Success));
    }

    [[nodiscard]] int error_count() const { return total() - success_count(); }

    [[nodiscard]] double success_rate() const {
        if (history_.empty()) return 0.0;
        return static_cast<double>(success_count()) / static_cast<double>(total()) * 100.0;
    }

    [[nodiscard]] std::map<StatusCode, int> frequency() const {
        std::map<StatusCode, int> freq;
        for (StatusCode c : history_) ++freq[c];
        return freq;
    }

    void print_history() const {
        std::cout << "\n--- Error History ---\n";
        for (StatusCode c : history_)
            std::cout << std::format("{}\n", to_string(c));
    }

    void print_statistics() const {
        std::cout << std::format(
            "\n--- Statistics ---\n"
            "Total events:  {}\n"
            "Success count: {}\n"
            "Error count:   {}\n"
            "Success rate:  {:.1f}%\n",
            total(), success_count(), error_count(), success_rate());
    }

    void print_frequency() const {
        std::cout << "\n--- Frequency Report ---\n";
        for (const auto& [code, count] : frequency())
            std::cout << std::format("{} -> {}\n", to_string(code), count);
    }
};

void print_divider() { std::cout << "-----------------------------\n"; }

int main() {
    ErrorTracker tracker;

    StatusCode status = perform_task();
    tracker.add(status);

    if (status != StatusCode::Success) {
        std::cout << std::format("Error code: {}\n", static_cast<int>(status));
        print_error(status);
        std::cout << std::format("Type:     {}\n", to_string(status));
        std::cout << std::format("Severity: {}\n", severity(status));
        log_error(status);

        if (is_recoverable(status)) {
            std::cout << "Retrying operation...\n";
            status = retry_task(3);
            tracker.add(status);
            std::cout << (status == StatusCode::Success
                         ? "Recovered successfully after retry.\n"
                         : "Retry failed.\n");
        }
    } else {
        std::cout << "Program exited successfully.\n";
    }

    print_divider();
    std::cout << "Testing additional error handling:\n";

    constexpr std::array simulated{
        StatusCode::FileError,
        StatusCode::NetworkError,
        StatusCode::MemoryError,
    };

    for (StatusCode code : simulated) {
        tracker.add(code);
        std::cout << std::format("\nCode: {}\n", static_cast<int>(code));
        print_error(code);
        std::cout << std::format("Recoverable? {}\n", is_recoverable(code) ? "Yes" : "No");
        std::cout << std::format("Severity:    {}\n", severity(code));
    }

    print_divider();
    tracker.print_history();

    std::cout << std::format("\nTotal tracked: {}\n", tracker.total());
    print_divider();

    if (const auto last = tracker.last())
        std::cout << std::format("Most recent: {}\n", to_string(*last));

    tracker.print_statistics();
    tracker.print_frequency();

    std::cout << "\n--- views pipeline over error history ---\n";
    std::vector<StatusCode> all_errors(simulated.begin(), simulated.end());
    const auto recoverable_count = std::ranges::count_if(all_errors, is_recoverable);
    std::cout << std::format("Recoverable errors: {}\n", recoverable_count);

    auto severities = all_errors
        | std::views::transform([](StatusCode c) { return severity(c); });
    std::cout << "Severities: ";
    for (std::string_view s : severities) std::cout << std::format("{} ", s);
    std::cout << '\n';

    print_divider();

    assert(to_string(StatusCode::Success)      == "SUCCESS");
    assert(to_string(StatusCode::FileError)    == "FILE_ERROR");
    assert(to_string(StatusCode::MemoryError)  == "MEMORY_ERROR");
    assert(severity(StatusCode::NetworkError)  == "MEDIUM");
    assert(!is_recoverable(StatusCode::FileError));
    assert(is_recoverable(StatusCode::NetworkError));
    assert(tracker.total() > 0);
    assert(tracker.last().has_value());

    std::cout << "\nAll assertions passed.\n";
    return status == StatusCode::Success ? EXIT_SUCCESS : EXIT_FAILURE;
}
