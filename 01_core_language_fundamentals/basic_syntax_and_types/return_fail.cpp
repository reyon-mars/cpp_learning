#include <iostream>
#include <cstdlib>

// ✅ ADDED: Custom status codes
enum StatusCode {
    SUCCESS = 0,
    FILE_ERROR = 1,
    NETWORK_ERROR = 2
};

// ✅ ADDED: Simulated operation
int performTask() {
    // simulate a failure (change value to test)
    return FILE_ERROR;
}

// ✅ ADDED: Error message helper
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

int main(void) {

    int status = -1;

    // ✅ ADDED: Use simulated task
    status = performTask();

    if (status != EXIT_SUCCESS) {
        std::cout << "Program exited with error code: "
                  << status << std::endl;

        // ✅ ADDED: Detailed error message
        printError(status);

    } else {
        std::cout << "Program exited successfully." << std::endl;
    }

    // Optional: normalize return value
    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
