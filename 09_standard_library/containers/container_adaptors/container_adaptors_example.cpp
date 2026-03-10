// Container Adaptors Exercise
// stack, queue, priority_queue

#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <functional>

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

    // ---- Additional small examples ----

    // stack size example
    s.push(7);
    s.push(8);
    std::cout << "Stack size: " << s.size() << "\n";

    // queue empty check
    std::cout << "Is queue empty? " << (q.empty() ? "Yes" : "No") << "\n";

    // min priority_queue
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_pq;
    min_pq.push(5);
    min_pq.push(10);
    min_pq.push(3);

    std::cout << "Min Priority Queue order: ";
    while (!min_pq.empty()) {
        std::cout << min_pq.top() << " ";
        min_pq.pop();
    }
    std::cout << "\n";

    // -----------------------------------

    return 0;
}
