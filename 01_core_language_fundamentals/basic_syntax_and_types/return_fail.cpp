#include <iostream>
#include <cstdlib>
#include <string>

// Custom status codes
enum StatusCode {
    SUCCESS = 0,
    FILE_ERROR = 1,
    NETWORK_ERROR = 2
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

int main(void) {

    int status = -1;

    status = performTask();

    if (status != EXIT_SUCCESS) {
        std::cout << "Program exited with error code: "
                  << status << std::endl;

        printError(status);

        std::cout << "Error type: " << errorToString(status) << "\n";

        if (isRecoverable(status)) {
            std::cout << "Retrying operation...\n";
            status = retryTask(3);

            if (status == SUCCESS) {
                std::cout << "Recovered successfully after retry.\n";
            } else {
                std::cout << "Retry failed.\n";
            }
        }

    } else {
        std::cout << "Program exited successfully." << std::endl;
    }

    // Optional: normalize return value
    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
