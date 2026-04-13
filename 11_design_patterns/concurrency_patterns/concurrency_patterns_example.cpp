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

    // -------- NEW ADDITION --------
    size_t size() const {
        std::lock_guard<std::mutex> lk(mut);
        return data.size();
    }
    // --------------------------------
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

// ---------------- Thread-safe logging ----------------
std::mutex cout_mutex;

void safe_print(const std::string& msg) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << msg << std::endl;
}
// -----------------------------------------------------

// ---------------- Producer / Consumer helpers ----------------

void producer(Queue<int>& q) {
    for (int i = 1; i <= 5; ++i) {
        q.push(i);
        safe_print("Produced: " + std::to_string(i));
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void consumer(Queue<int>& q) {
    int value;
    for (int i = 0; i < 5; ++i) {
        while (!q.try_pop(value)) {
            std::this_thread::yield();
        }
        safe_print("Consumed: " + std::to_string(value));
    }
}

// -------- NEW ADDITIONS --------

// Timed consumer (tries for limited time)
void timed_consumer(Queue<int>& q) {
    int value;
    for (int i = 0; i < 5; ++i) {
        auto start = std::chrono::steady_clock::now();
        while (!q.try_pop(value)) {
            if (std::chrono::steady_clock::now() - start >
                std::chrono::milliseconds(200)) {
                safe_print("Timed consumer: timeout");
                break;
            }
        }
        if (q.size() >= 0)
            safe_print("Timed consumer checked queue");
    }
}

// Singleton access from multiple threads
void singleton_task() {
    auto s = ThreadSafeSingleton::get_instance();
    safe_print("Singleton accessed from thread");
}

// -----------------------------------------------------

// ---------------- Main ----------------

int main() {

    // Singleton demo
    auto singleton = ThreadSafeSingleton::get_instance();
    std::cout << "Singleton acquired\n";

    // -------- NEW: multiple threads using singleton --------
    std::thread s1(singleton_task);
    std::thread s2(singleton_task);
    s1.join();
    s2.join();
    // ------------------------------------------------------

    // Producer-consumer demo
    Queue<int> q;
    std::thread p(producer, std::ref(q));
    std::thread c(consumer, std::ref(q));

    p.join();
    c.join();

    std::cout << "\n";

    // -------- NEW: additional consumer --------
    std::thread p2(producer, std::ref(q));
    std::thread tc(timed_consumer, std::ref(q));

    p2.join();
    tc.join();
    // -----------------------------------------

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

    // -------- NEW: read-heavy workload --------
    std::cout << "\nRead-heavy workload:\n";

    std::vector<std::thread> readers;
    for (int i = 0; i < 5; ++i) {
        readers.emplace_back([&]{
            data.read();
        });
    }

    std::thread writer2([&]{
        data.write(20);
    });

    for (auto& t : readers) t.join();
    writer2.join();
    // -----------------------------------------

    return 0;
}
