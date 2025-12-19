// ======================================================
// EXTRA CODE (append only)
// ======================================================

void log_msg_fast(const char* tag, const char* msg)
{
    std::cout << '[' << tag << ']' << msg << '\n';
}

void test_fast_logger() {
    scoped_timer timer_fast("C-string Logger (Total)");

    for (int i = 0; i < 500000; ++i) {
        log_msg_fast("FAST", "Logging quickly!");
    }
}

struct LoggerBenchmark {
    LoggerBenchmark() {
        std::cout << "\n=== Running Extra Benchmark ===\n";
        test_fast_logger();
    }
};

// Automatically runs before main()
LoggerBenchmark __auto_logger_bench;
