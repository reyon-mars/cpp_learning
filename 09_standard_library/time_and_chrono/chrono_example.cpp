// Chrono Exercise
// Duration, time_point, clocks

#include <iostream>
#include <chrono>
#include <thread>

int main() {
    using namespace std::chrono_literals;
    
    // Duration
    auto duration = 5s;
    std::cout << "Duration: " << duration.count() << " seconds\n";
    
    // High-resolution clock
    auto start = std::chrono::high_resolution_clock::now();
    std::this_thread::sleep_for(100ms);
    auto end = std::chrono::high_resolution_clock::now();
    
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Elapsed: " << elapsed.count() << " ms\n";
    
    return 0;
}
