// Concurrency Patterns Exercise
// Thread-safe singleton, producer-consumer, read-write lock

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

// Thread-safe singleton
class ThreadSafeSingleton {
private:
    static std::mutex mutex;
    static ThreadSafeSingleton* instance;
    
    ThreadSafeSingleton() {}
    
public:
    static ThreadSafeSingleton* get_instance() {
        std::lock_guard<std::mutex> lock(mutex);
        if (!instance) {
            instance = new ThreadSafeSingleton();
        }
        return instance;
    }
};

std::mutex ThreadSafeSingleton::mutex;
ThreadSafeSingleton* ThreadSafeSingleton::instance = nullptr;

// Producer-consumer
template<typename T>
class Queue {
private:
    std::queue<T> data;
    mutable std::mutex mut;
    
public:
    void push(T value) {
        std::lock_guard<std::mutex> lk(mut);
        data.push(value);
    }
    
    bool try_pop(T& value) {
        std::lock_guard<std::mutex> lk(mut);
        if (data.empty()) return false;
        value = data.front();
        data.pop();
        return true;
    }
};

int main() {
    auto singleton = ThreadSafeSingleton::get_instance();
    std::cout << "Singleton acquired\n";
    
    return 0;
}
