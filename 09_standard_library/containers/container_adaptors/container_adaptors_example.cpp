// Container Adaptors Exercise
// stack, queue, priority_queue

#include <iostream>
#include <stack>
#include <queue>

int main() {
    // Stack
    std::stack<int> s;
    s.push(1);
    s.push(2);
    s.push(3);
    
    while (!s.empty()) {
        std::cout << "Stack: " << s.top() << "\n";
        s.pop();
    }
    
    // Queue
    std::queue<int> q;
    q.push(10);
    q.push(20);
    q.push(30);
    
    while (!q.empty()) {
        std::cout << "Queue: " << q.front() << "\n";
        q.pop();
    }
    
    // Priority queue
    std::priority_queue<int> pq;
    pq.push(5);
    pq.push(10);
    pq.push(3);
    
    while (!pq.empty()) {
        std::cout << "PQ: " << pq.top() << "\n";
        pq.pop();
    }
    
    return 0;
}
