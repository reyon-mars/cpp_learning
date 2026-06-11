#include <coroutine>
#include <exception>
#include <iostream>
#include <vector>
#include <string_view>
#include <utility>

class SimpleCoroutine {
public:
    struct promise_type {
        int value = 0;

        SimpleCoroutine get_return_object() {
            return SimpleCoroutine{Handle::from_promise(*this)};
        }

        std::suspend_never  initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend()   noexcept { return {}; }

        void return_value(int v) noexcept { value = v; }
        void unhandled_exception() noexcept { std::terminate(); }
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit SimpleCoroutine(Handle h) noexcept : handle_{h} {}

    SimpleCoroutine(const SimpleCoroutine&)            = delete;
    SimpleCoroutine& operator=(const SimpleCoroutine&) = delete;

    SimpleCoroutine(SimpleCoroutine&& other) noexcept
        : handle_{std::exchange(other.handle_, {})} {}

    SimpleCoroutine& operator=(SimpleCoroutine&& other) noexcept {
        if (this != &other) {
            if (handle_) handle_.destroy();
            handle_ = std::exchange(other.handle_, {});
        }
        return *this;
    }

    ~SimpleCoroutine() { if (handle_) handle_.destroy(); }

    [[nodiscard]] int  get()  const noexcept { return handle_.promise().value; }
    [[nodiscard]] bool done() const noexcept { return !handle_ || handle_.done(); }

    void resume() {
        if (!done()) handle_.resume();
        else         std::cout << "[already done]\n";
    }

private:
    Handle handle_;
};

void print_status(std::string_view name, const SimpleCoroutine& c) {
    std::cout << name << " done=" << std::boolalpha << c.done() << "\n";
}

SimpleCoroutine compute_value() {
    std::cout << "Coroutine started\n";
    co_return 42;
}

SimpleCoroutine suspended_example() {
    std::cout << "Before suspension\n";
    co_await std::suspend_always{};
    std::cout << "After resume\n";
    co_return 100;
}

SimpleCoroutine multi_suspend() {
    std::cout << "Step 1\n"; co_await std::suspend_always{};
    std::cout << "Step 2\n"; co_await std::suspend_always{};
    std::cout << "Step 3\n";
    co_return 300;
}

SimpleCoroutine quick_task() {
    co_return 7 * 6;
}

SimpleCoroutine auto_resume_task() {
    std::cout << "Auto task start\n";
    co_return 555;
}

SimpleCoroutine counting_coroutine(int limit) {
    for (int i = 1; i <= limit; ++i) {
        std::cout << "Counter=" << i << "\n";
        co_await std::suspend_always{};
    }
    co_return limit;
}

SimpleCoroutine vector_sum_coroutine(const std::vector<int>& values) {
    int total = 0;
    for (int v : values) {
        total += v;
        std::cout << "Adding " << v << " running_total=" << total << "\n";
        co_await std::suspend_always{};
    }
    co_return total;
}

SimpleCoroutine square_coroutine(int value) {
    std::cout << "Squaring " << value << "\n";
    co_return value * value;
}

SimpleCoroutine chained_coroutine() {
    std::cout << "Chain start\n";
    co_await std::suspend_always{};
    std::cout << "Chain continue\n";
    co_return 999;
}

int main() {
    std::cout << "=== compute_value ===\n";
    auto c1 = compute_value();
    std::cout << "Result=" << c1.get() << "\n";

    std::cout << "\n=== suspended_example ===\n";
    auto c2 = suspended_example();
    print_status("c2", c2);
    c2.resume();
    std::cout << "Result=" << c2.get() << "\n";
    print_status("c2", c2);

    std::cout << "\n=== multi_suspend ===\n";
    auto c3 = multi_suspend();
    c3.resume();
    c3.resume();
    c3.resume();
    std::cout << "Result=" << c3.get() << "\n";
    c3.resume();

    std::cout << "\n=== quick_task ===\n";
    auto c4 = quick_task();
    std::cout << "Result=" << c4.get() << "\n";

    std::cout << "\n=== auto_resume_task ===\n";
    auto c5 = auto_resume_task();
    std::cout << "Result=" << c5.get() << "\n";

    std::cout << "\n=== counting_coroutine(3) ===\n";
    auto counter = counting_coroutine(3);
    print_status("counter", counter);
    counter.resume();
    counter.resume();
    counter.resume();
    std::cout << "Result=" << counter.get() << "\n";
    print_status("counter", counter);

    std::cout << "\n=== vector_sum_coroutine ===\n";
    const std::vector<int> nums{1, 2, 3, 4};
    auto vsum = vector_sum_coroutine(nums);
    for (std::size_t i = 0; i < nums.size(); ++i) vsum.resume();
    std::cout << "Result=" << vsum.get() << "\n";

    std::cout << "\n=== square_coroutine(9) ===\n";
    auto sq = square_coroutine(9);
    std::cout << "Result=" << sq.get() << "\n";

    std::cout << "\n=== chained_coroutine ===\n";
    auto chain = chained_coroutine();
    print_status("chain", chain);
    chain.resume();
    std::cout << "Result=" << chain.get() << "\n";
    print_status("chain", chain);

    return 0;
}
