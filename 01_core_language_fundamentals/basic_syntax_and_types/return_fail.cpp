#include <iostream>
#include <cstdlib>

int main(void) {

    int status = -1;

    if (status != EXIT_SUCCESS) {
        std::cout << "Program exited with error code: "
                  << status << std::endl;
    } else {
        std::cout << "Program exited successfully." << std::endl;
    }

    // Optional: normalize return value
    return (status == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
