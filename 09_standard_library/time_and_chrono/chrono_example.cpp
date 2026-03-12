// Chrono Exercise
// Duration, time_point, clocks

#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>

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

    // ---- Additional small examples ----

    // steady_clock example
    auto steady_start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(50ms);
    auto steady_end = std::chrono::steady_clock::now();

    auto steady_elapsed =
        std::chrono::duration_cast<std::chrono::microseconds>(steady_end - steady_start);

    std::cout << "Steady clock elapsed: "
              << steady_elapsed.count() << " us\n";

    // system_clock current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);

    std::cout << "Current system time: "
              << std::ctime(&now_time);

    // duration conversion
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(elapsed);
    std::cout << "Elapsed in seconds: "
              << seconds.count() << " s\n";

    // -----------------------------------

    return 0;
}
