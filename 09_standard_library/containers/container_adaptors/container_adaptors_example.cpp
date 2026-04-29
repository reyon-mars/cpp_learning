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
    // EXTRA SMALL ADDITIONS
    // -----------------------------------

    // stack top without popping
    s.push(100);
    std::cout << "Top element of stack: " << s.top() << "\n";

    // queue front and back
    q.push(40);
    q.push(50);
    std::cout << "Queue front: " << q.front() << "\n";
    std::cout << "Queue back: " << q.back() << "\n";

    // priority_queue size check
    pq.push(15);
    pq.push(1);
    std::cout << "Priority queue size: " << pq.size() << "\n";

    // swap two queues
    std::queue<int> q2;
    q2.push(99);
    q.swap(q2);
    std::cout << "After swap, q front: " << q.front() << "\n";

    // emplace example (construct in-place)
    s.emplace(200);
    std::cout << "After emplace, stack top: " << s.top() << "\n";

    // ---------------- NEW SMALL ADDITIONS ----------------

    // stack pop + check
    if (!s.empty()) {
        s.pop();
        std::cout << "After pop, new top: " << s.top() << "\n";
    }

    // queue size after operations
    std::cout << "Queue size: " << q.size() << "\n";

    // priority_queue top without popping
    if (!pq.empty()) {
        std::cout << "Top of priority queue: " << pq.top() << "\n";
    }

    // emplace in queue
    q.emplace(77);
    std::cout << "After emplace, queue back: " << q.back() << "\n";

    // clear queue manually
    while (!q.empty()) q.pop();
    std::cout << "Queue cleared, is empty? "
              << (q.empty() ? "Yes" : "No") << "\n";

    // ----------------------------------------------------

    return 0;
}
