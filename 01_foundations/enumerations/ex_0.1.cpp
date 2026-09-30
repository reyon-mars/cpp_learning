#include <iostream>

// Traditional enum (values leak into the surrounding scope)
enum TrafficLight {
    RED,
    YELLOW,
    GREEN
};

void printLightStatus(TrafficLight light) {
    switch (light) {
        case RED:
            std::cout << "Stop!\n";
            break;
        case YELLOW:
            std::cout << "Caution!\n";
            break;
        case GREEN:
            std::cout << "Go!\n";
            break;
    }
}

int main() {
    TrafficLight current = RED;
    printLightStatus(current); // Outputs: Stop!
    return 0;
}
