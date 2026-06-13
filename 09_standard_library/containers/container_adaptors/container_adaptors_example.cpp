#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <deque>
#include <string>
#include <functional>

void drain_stack(std::stack<int>& s, std::string_view label) {
    std::cout << label << ": ";
    while (!s.empty()) { std::cout << s.top() << " "; s.pop(); }
    std::cout << "\n";
}

void drain_queue(std::queue<int>& q, std::string_view label) {
    std::cout << label << ": ";
    while (!q.empty()) { std::cout << q.front() << " "; q.pop(); }
    std::cout << "\n";
}

template<typename PQ>
void drain_pq(PQ& pq, std::string_view label) {
    std::cout << label << ": ";
    while (!pq.empty()) { std::cout << pq.top() << " "; pq.pop(); }
    std::cout << "\n";
}

int main() {
    std::cout << "=== stack<int> ===\n";
    std::stack<int> s;
    s.push(1); s.push(2); s.push(3);
    drain_stack(s, "LIFO");

    s.push(7); s.push(8);
    std::cout << "size=" << s.size() << " top=" << s.top() << "\n";

    s.emplace(200);
    std::cout << "After emplace top=" << s.top() << "\n";
    s.pop();
    std::cout << "After pop top=" << s.top() << "\n";

    for (int i = 1; i <= 3; ++i) s.push(i * 10);
    drain_stack(s, "Loop pushes");

    std::cout << "\n=== queue<int> ===\n";
    std::queue<int> q;
    q.push(10); q.push(20); q.push(30);
    drain_queue(q, "FIFO");

    std::cout << "empty=" << std::boolalpha << q.empty() << "\n";

    q.push(40); q.push(50);
    std::cout << "front=" << q.front() << " back=" << q.back() << " size=" << q.size() << "\n";

    q.emplace(77);
    std::cout << "After emplace back=" << q.back() << "\n";

    std::queue<int> q2;
    q2.push(99);
    q.swap(q2);
    std::cout << "After swap front=" << q.front() << "\n";
    q.swap(q2);

    while (!q.empty()) q.pop();
    std::cout << "Cleared empty=" << q.empty() << "\n";

    q.push(1); q.push(2); q.push(3);
    drain_queue(q, "Refilled");

    std::cout << "\n=== queue<string> ===\n";
    std::queue<std::string> names;
    names.push("Alice"); names.push("Bob");
    std::cout << "front=" << names.front() << "\n";

    std::cout << "\n=== priority_queue<int> (max-heap) ===\n";
    std::priority_queue<int> pq;
    pq.push(5); pq.push(10); pq.push(3);
    drain_pq(pq, "Max-heap");

    pq.push(100); pq.push(50); pq.push(200);
    std::cout << "top=" << pq.top() << " size=" << pq.size() << "\n";

    pq.push(-1); pq.push(-5); pq.push(2);
    drain_pq(pq, "With negatives");

    std::cout << "\n=== priority_queue<int, vector, greater<>> (min-heap) ===\n";
    std::priority_queue<int, std::vector<int>, std::greater<>> min_pq;
    min_pq.push(5); min_pq.push(10); min_pq.push(3);
    drain_pq(min_pq, "Min-heap");

    std::cout << "\n=== stack with deque backing ===\n";
    const std::deque<int> dq{11, 22, 33};
    std::stack<int, std::deque<int>> custom_stack{dq};
    std::cout << "top=" << custom_stack.top() << "\n";
    drain_stack(custom_stack, "Custom-stack");

    return 0;
}
