#include <iostream>
#include <cstdlib>
#include <string>

// ✅ ADDED
#include <vector>
#include <ctime>

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
};

// ✅ ADDED: Divider helper

void printDivider() {
    std::cout << "-----------------------------\n";
}

// ------------------------------------------------

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

    for (int code : simulatedErrors) {

        tracker.add(code);

        std::cout << "\nCode: " << code << "\n";

        printError(code);

        std::cout << "Recoverable? "
                  << (isRecoverable(code) ? "Yes" : "No")
                  << "\n";
    }

    printDivider();

    tracker.printHistory();

    std::cout << "\nTotal tracked errors: "
              << tracker.total() << "\n";

    // ------------------------------------------------

    // Optional: normalize return value
    return (status == 0)
           ? EXIT_SUCCESS
           : EXIT_FAILURE;
}
