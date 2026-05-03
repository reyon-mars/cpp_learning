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

    // ===== VERY SMALL NEW ADDITIONS =====

    // Measure nanoseconds
    auto nano_elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    std::cout << "Elapsed in nanoseconds: "
              << nano_elapsed.count() << " ns\n";

    // Compare two durations
    if (elapsed > 50ms)
        std::cout << "Elapsed time is greater than 50ms\n";

    // Add durations
    auto total_time = elapsed + std::chrono::milliseconds(50);
    std::cout << "Elapsed + 50ms: "
              << total_time.count() << " ms\n";

    // Simple loop timing
    auto loop_start = std::chrono::high_resolution_clock::now();
    for (volatile int i = 0; i < 1000000; ++i);
    auto loop_end = std::chrono::high_resolution_clock::now();

    auto loop_time =
        std::chrono::duration_cast<std::chrono::microseconds>(loop_end - loop_start);

    std::cout << "Loop execution time: "
              << loop_time.count() << " us\n";

    // ---- EXTRA SMALL ADDITIONS ----

    // Check clock properties
    std::cout << "Is steady_clock steady? "
              << (std::chrono::steady_clock::is_steady ? "Yes" : "No") << "\n";

    // Time since epoch (system_clock)
    auto since_epoch = now.time_since_epoch();
    auto seconds_epoch =
        std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
    std::cout << "Seconds since epoch: "
              << seconds_epoch.count() << "\n";

    // Sleep for a very small duration
    std::this_thread::sleep_for(10ms);
    std::cout << "Slept for additional 10ms\n";

    // Average of two durations (simple example)
    auto avg_time = (elapsed + std::chrono::milliseconds(50)) / 2;
    std::cout << "Average duration: "
              << avg_time.count() << " ms\n";

    // --------------------------------

    return 0;
}
