// Concurrency Patterns Exercise
// Thread-safe singleton, producer-consumer, read-write lock

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <shared_mutex>
#include <vector>
#include <chrono>

// ---------------- Thread-safe singleton ----------------

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

// ---------------- Producer-consumer queue ----------------

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

// ---------------- Read-write lock example ----------------

class SharedData {
private:
    int value = 0;
    std::shared_mutex rw_mutex;

public:
    void write(int v) {
        std::unique_lock lock(rw_mutex);
        value = v;
        std::cout << "Writer updated value to " << value << "\n";
    }

    void read() const {
        std::shared_lock lock(rw_mutex);
        std::cout << "Reader saw value " << value << "\n";
    }
};

// ---------------- Producer / Consumer helpers ----------------

void producer(Queue<int>& q) {
    for (int i = 1; i <= 5; ++i) {
        q.push(i);
        std::cout << "Produced: " << i << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void consumer(Queue<int>& q) {
    int value;
    for (int i = 0; i < 5; ++i) {
        while (!q.try_pop(value)) {
            std::this_thread::yield();
        }
        std::cout << "Consumed: " << value << "\n";
    }
}

// ---------------- Main ----------------

int main() {

    // Singleton demo
    auto singleton = ThreadSafeSingleton::get_instance();
    std::cout << "Singleton acquired\n";

    // Producer-consumer demo
    Queue<int> q;
    std::thread p(producer, std::ref(q));
    std::thread c(consumer, std::ref(q));

    p.join();
    c.join();

    std::cout << "\n";

    // Read-write lock demo
    SharedData data;

    std::thread writer([&]{
        data.write(10);
    });

    std::thread reader1([&]{
        data.read();
    });

    std::thread reader2([&]{
        data.read();
    });

    writer.join();
    reader1.join();
    reader2.join();

    return 0;
}
 
