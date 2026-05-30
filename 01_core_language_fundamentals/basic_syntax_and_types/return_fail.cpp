#include <iostream>
#include <cstdlib>
#include <string>

// ✅ ADDED
#include <vector>
#include <ctime>
#include <map>

// Custom status codes
enum StatusCode {
    SUCCESS = 0,
    FILE_ERROR = 1,
    NETWORK_ERROR = 2
};

// ✅ ADDED: Extra error type
enum ExtendedStatusCode {
    MEMORY_ERROR = 3
};

// Simulated operation
int performTask() {
    // simulate a failure (change value to test)
    return FILE_ERROR;
}

// Error message helper
void printError(int code) {
    switch (code) {
        case SUCCESS:
            std::cout << "No error.\n";
            break;
        case FILE_ERROR:
            std::cout << "File error occurred.\n";
            break;
        case NETWORK_ERROR:
            std::cout << "Network error occurred.\n";
            break;

        // ✅ ADDED
        case MEMORY_ERROR:
            std::cout << "Memory error occurred.\n";
            break;

        default:
            std::cout << "Unknown error.\n";
    }
}


// Convert error code to string (useful for logs)
std::string errorToString(int code) {
    switch (code) {
        case SUCCESS: return "SUCCESS";
        case FILE_ERROR: return "FILE_ERROR";
        case NETWORK_ERROR: return "NETWORK_ERROR";

        // ✅ ADDED
        case MEMORY_ERROR: return "MEMORY_ERROR";

        default: return "UNKNOWN_ERROR";
    }
}

// Check if error is recoverable
bool isRecoverable(int code) {
    return code == NETWORK_ERROR; // example: network can be retried
}

// Retry mechanism (simple demo)
int retryTask(int attempts) {
    int result;

    for (int i = 1; i <= attempts; ++i) {

        std::cout << "Attempt " << i << "...\n";

        result = performTask();

        if (result == SUCCESS) {
            return SUCCESS;
        }
    }

    return result;
}

// ------------------------------------------------
// ✅ ADDED: Log helper

void logError(int code) {
    std::time_t now = std::time(nullptr);

    std::cout << "[LOG " << now << "] "
              << errorToString(code) << "\n";
}

// ✅ ADDED: Error history tracker

class ErrorTracker {
private:
    std::vector<int> history;

public:
    void add(int code) {
        history.push_back(code);
    }

    void printHistory() const {

        std::cout << "\n--- Error History ---\n";

        for (int code : history) {
            std::cout << errorToString(code) << "\n";
        }
    }

    int total() const {
        return history.size();
    }

    // ✅ NEW
    int successCount() const {
        int count = 0;
        for (int code : history) {
            if (code == SUCCESS)
                ++count;
        }
        return count;
    }

    // ✅ NEW
    int errorCount() const {
        return total() - successCount();
    }

    // ✅ NEW
    int lastError() const {
        if (history.empty())
            return SUCCESS;

        return history.back();
    }
};

// ✅ ADDED: Divider helper

void printDivider() {
    std::cout << "-----------------------------\n";
}

// =====================================================
// ✅ NEW ADDITIONS
// =====================================================

// Error severity
std::string errorSeverity(int code) {

    switch (code) {

        case SUCCESS:
            return "NONE";

        case FILE_ERROR:
            return "LOW";

        case NETWORK_ERROR:
            return "MEDIUM";

        case MEMORY_ERROR:
            return "HIGH";

        default:
            return "UNKNOWN";
    }
}

// Print error summary statistics
void printStatistics(const ErrorTracker& tracker) {

    std::cout << "\n--- Statistics ---\n";

    std::cout << "Total events: "
              << tracker.total() << "\n";

    std::cout << "Success count: "
              << tracker.successCount() << "\n";

    std::cout << "Error count: "
              << tracker.errorCount() << "\n";

    if (tracker.total() > 0) {

        double successRate =
            (static_cast<double>(tracker.successCount()) /
             tracker.total()) * 100.0;

        std::cout << "Success rate: "
                  << successRate << "%\n";
    }
}

// Frequency report
void printFrequencyReport(const std::vector<int>& codes) {

    std::map<int, int> freq;

    for (int code : codes) {
        ++freq[code];
    }

    std::cout << "\n--- Frequency Report ---\n";

    for (const auto& entry : freq) {

        std::cout
            << errorToString(entry.first)
            << " -> "
            << entry.second
            << "\n";
    }
}

// =====================================================

int main(void) {

    int status = -1;

    // ✅ ADDED
    ErrorTracker tracker;

    status = performTask();

    // ✅ ADDED
    tracker.add(status);

    if (status != EXIT_SUCCESS) {

        std::cout << "Program exited with error code: "
                  << status << std::endl;

        printError(status);

        std::cout << "Error type: "
                  << errorToString(status) << "\n";

        // ✅ ADDED
        logError(status);

        // ✅ NEW
        std::cout << "Severity: "
                  << errorSeverity(status)
                  << "\n";

        if (isRecoverable(status)) {

            std::cout << "Retrying operation...\n";

            status = retryTask(3);

            // ✅ ADDED
            tracker.add(status);

            if (status == SUCCESS) {

                std::cout << "Recovered successfully after retry.\n";

            } else {

                std::cout << "Retry failed.\n";
            }
        }

    } else {

        std::cout << "Program exited successfully." << std::endl;
    }

    // ------------------------------------------------
    // ✅ ADDED: Extra demonstrations

    printDivider();

    std::cout << "Testing additional error handling:\n";

    int simulatedErrors[] = {
        FILE_ERROR,
        NETWORK_ERROR,
        MEMORY_ERROR
    };

    // ✅ NEW
    std::vector<int> allCodes;

    for (int code : simulatedErrors) {

        tracker.add(code);
        allCodes.push_back(code);

        std::cout << "\nCode: " << code << "\n";

        printError(code);

        std::cout << "Recoverable? "
                  << (isRecoverable(code) ? "Yes" : "No")
                  << "\n";

        // ✅ NEW
        std::cout << "Severity: "
                  << errorSeverity(code)
                  << "\n";
    }

    printDivider();

    tracker.printHistory();

    std::cout << "\nTotal tracked errors: "
              << tracker.total() << "\n";

    // =================================================
    // ✅ NEW REPORTING FEATURES
    // =================================================

    printDivider();

    std::cout << "Most recent status: "
              << errorToString(tracker.lastError())
              << "\n";

    printStatistics(tracker);

    printFrequencyReport(allCodes);

    printDivider();

    // ------------------------------------------------

    // Optional: normalize return value
    return (status == 0)
           ? EXIT_SUCCESS
           : EXIT_FAILURE;
}
