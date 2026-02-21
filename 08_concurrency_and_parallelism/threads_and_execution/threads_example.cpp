// Threads and Execution Exercise
// std::thread basics and thread management

#include <iostream>
#include <thread>
#include <vector>

void worker_function(int id) {
    for (int i = 0; i < 3; ++i) {
        std::cout << "Thread " << id << " iteration " << i << "\n";
    }
}

int main() {
    std::vector<std::thread> threads;
    
    // Create threads
    for (int i = 0; i < 3; ++i) {
        threads.emplace_back(worker_function, i);
    }
    
    // Join all threads
    for (auto& t : threads) {
        t.join();
    }
    
    std::cout << "All threads completed\n";
    
    return 0;
}
