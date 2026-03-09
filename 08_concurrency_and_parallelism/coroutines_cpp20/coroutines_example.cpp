// Coroutines Exercise (C++20)
// stackless coroutines and co_await

#include <iostream>
#include <coroutine>

// Simplified coroutine example for C++20
class SimpleCoroutine {
public:
    struct promise_type {
        int value;

        SimpleCoroutine get_return_object() {
            return SimpleCoroutine(
                std::coroutine_handle<promise_type>::from_promise(*this)
            );
        }

        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }

        void unhandled_exception() {
            std::terminate();
        }

        void return_value(int v) { value = v; }
    };

    std::coroutine_handle<promise_type> handle;

    SimpleCoroutine(std::coroutine_handle<promise_type> h) : handle(h) {}

    ~SimpleCoroutine() {
        if (handle)
            handle.destroy();
    }

    int get() const {
        return handle.promise().value;
    }

    void resume() {
        if (!handle.done())
            handle.resume();
    }
};

// Simple coroutine function
SimpleCoroutine compute_value() {
    std::cout << "Coroutine started\n";
    co_return 42;
}

// Coroutine with suspension
SimpleCoroutine suspended_example() {
    std::cout << "Before suspension\n";
    co_await std::suspend_always{};
    std::cout << "After resume\n";
    co_return 100;
}

// Main
int main() {

    std::cout << "C++20 Coroutines support\n";

    auto coro = compute_value();
    std::cout << "Returned value: " << coro.get() << "\n";

    std::cout << "\nSuspension demo\n";

    auto coro2 = suspended_example();
    std::cout << "Resuming coroutine...\n";
    coro2.resume();

    std::cout << "Returned value: " << coro2.get() << "\n";

    return 0;
}
