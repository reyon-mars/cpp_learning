#include <iostream>
#include <cstdlib>

int main(void) {

    int status = -1;

    if (status != 0) {
        std::cout << "Program exited with error code: "
                  << status << std::endl;
    } else {
        std::cout << "Program exited successfully." << std::endl;
    }

    return status;
}
