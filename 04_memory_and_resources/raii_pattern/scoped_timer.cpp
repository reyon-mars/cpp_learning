#include <iostream>
#include <chrono>
#include <string>
#include <thread>

class scoped_timer {
    
private:
    std::chrono::steady_clock::time_point start;
    std::string name;
    bool stopped = false;

public:
    explicit scoped_timer(std::string str)
        : start(std::chrono::steady_clock::now()),
          name(std::move(str)) {}

    // Prevent copying
    scoped_timer(const scoped_timer&) = delete;
    scoped_timer& operator=(const scoped_timer&) = delete;

    // Allow moving
    scoped_timer(scoped_timer&& other) noexcept
        : start(other.start),
          name(std::move(other.name)),
          stopped(other.stopped)
    {
        other.stopped = true;
    }

    // Manual stop
    void stop() {
        if (!stopped) {
            auto end = std::chrono::steady_clock::now();
            auto duration =
                std::chrono::duration_cast<std::chrono::microseconds>(
                    end - start).count();

            std::cout << name << ": "
                      << duration << " us\n";

            stopped = true;
        }
    }

    ~scoped_timer() {
        stop();
    }
};

// ---------------- Example Usage ----------------

int main() {

    {
        scoped_timer timer("Sleep 100ms");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    {
        scoped_timer timer("Manual stop example");
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        timer.stop();  // stops early
    }

    return 0;
}
