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
            return SimpleCoroutine(std::coroutine_handle<promise_type>::from_promise(*this));
        }
        
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() { return {}; }
        
        void unhandled_exception() {}
        
        void return_value(int v) { value = v; }
    };
    
    std::coroutine_handle<promise_type> handle;
    
    SimpleCoroutine(std::coroutine_handle<promise_type> h) : handle(h) {}
};

// Example generator coroutine (conceptual)
int main() {
    std::cout << "C++20 Coroutines support\n";
    return 0;
}
