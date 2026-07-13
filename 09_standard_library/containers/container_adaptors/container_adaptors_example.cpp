#include <iostream>
#include <stack>
#include <queue>
#include <vector>
#include <deque>
#include <string>
#include <functional>
#include <format>
#include <ranges>
#include <cassert>
#include <concepts>
#include <optional>
#include <algorithm>

template <typename C>
concept Drainable = requires(C c) {
    { c.empty() } -> std::same_as<bool>;
    { c.top()   };
    c.pop();
};

template <typename C>
concept FrontDrainable = requires(C c) {
    { c.empty()  } -> std::same_as<bool>;
    { c.front()  };
    c.pop();
};

template <Drainable C>
void drain_top(C& c, std::string_view label) {
    std::cout << std::format("{}: ", label);
    while (!c.empty()) { std::cout << std::format("{} ", c.top()); c.pop(); }
    std::cout << '\n';
}

template <FrontDrainable C>
void drain_front(C& c, std::string_view label) {
    std::cout << std::format("{}: ", label);
    while (!c.empty()) { std::cout << std::format("{} ", c.front()); c.pop(); }
    std::cout << '\n';
}

template <Drainable PQ>
[[nodiscard]] std::vector<typename PQ::value_type> drain_to_vec(PQ pq) {
    std::vector<typename PQ::value_type> result;
    result.reserve(pq.size());
    while (!pq.empty()) { result.push_back(pq.top()); pq.pop(); }
    return result;
}

template <std::ranges::input_range R>
[[nodiscard]] std::stack<std::ranges::range_value_t<R>>
range_to_stack(R&& r) {
    std::stack<std::ranges::range_value_t<R>> s;
    for (auto&& v : r) s.push(std::forward<decltype(v)>(v));
    return s;
}

template <std::ranges::input_range R>
[[nodiscard]] std::queue<std::ranges::range_value_t<R>>
range_to_queue(R&& r) {
    std::queue<std::ranges::range_value_t<R>> q;
    for (auto&& v : r) q.push(std::forward<decltype(v)>(v));
    return q;
}

template <std::ranges::input_range R,
          typename Comp = std::less<std::ranges::range_value_t<R>>>
[[nodiscard]] auto range_to_pq(R&& r, Comp cmp = {}) {
    using T = std::ranges::range_value_t<R>;
    std::priority_queue<T, std::vector<T>, Comp> pq(cmp);
    for (auto&& v : r) pq.push(std::forward<decltype(v)>(v));
    return pq;
}

int main() {
    std::cout << "=== stack<int> ===\n";
    std::stack<int> s;
    s.push(1); s.push(2); s.push(3);
    drain_top(s, "LIFO");

    s.push(7); s.push(8);
    std::cout << std::format("size={} top={}\n", s.size(), s.top());

    s.emplace(200);
    std::cout << std::format("After emplace top={}\n", s.top());
    s.pop();
    std::cout << std::format("After pop top={}\n", s.top());

    for (int i : std::views::iota(1, 4)) s.push(i * 10);
    drain_top(s, "Loop pushes");

    std::cout << "\n=== queue<int> ===\n";
    std::queue<int> q;
    q.push(10); q.push(20); q.push(30);
    drain_front(q, "FIFO");
    std::cout << std::format("empty={}\n", q.empty());

    q.push(40); q.push(50);
    std::cout << std::format("front={} back={} size={}\n", q.front(), q.back(), q.size());

    q.emplace(77);
    std::cout << std::format("After emplace back={}\n", q.back());

    std::queue<int> q2;
    q2.push(99);
    q.swap(q2);
    std::cout << std::format("After swap front={}\n", q.front());
    q.swap(q2);

    while (!q.empty()) q.pop();
    std::cout << std::format("Cleared empty={}\n", q.empty());

    q.push(1); q.push(2); q.push(3);
    drain_front(q, "Refilled");

    std::cout << "\n=== queue<string> ===\n";
    std::queue<std::string> names;
    names.push("Alice"); names.push("Bob");
    std::cout << std::format("front={}\n", names.front());

    std::cout << "\n=== priority_queue<int> (max-heap) ===\n";
    std::priority_queue<int> pq;
    pq.push(5); pq.push(10); pq.push(3);
    drain_top(pq, "Max-heap");

    pq.push(100); pq.push(50); pq.push(200);
    std::cout << std::format("top={} size={}\n", pq.top(), pq.size());

    pq.push(-1); pq.push(-5); pq.push(2);
    drain_top(pq, "With negatives");

    std::cout << "\n=== priority_queue<int, vector, greater<>> (min-heap) ===\n";
    std::priority_queue<int, std::vector<int>, std::greater<>> min_pq;
    min_pq.push(5); min_pq.push(10); min_pq.push(3);
    drain_top(min_pq, "Min-heap");

    std::cout << "\n=== stack with deque backing ===\n";
    const std::deque<int> dq{11, 22, 33};
    std::stack<int, std::deque<int>> custom_stack{dq};
    std::cout << std::format("top={}\n", custom_stack.top());
    drain_top(custom_stack, "Custom-stack");

    std::cout << "\n=== range_to_stack / range_to_queue / range_to_pq ===\n";
    {
        constexpr std::array vals{4, 2, 7, 1, 5};
        auto from_range_s = range_to_stack(vals);
        std::cout << std::format("range_to_stack top={} size={}\n",
                                 from_range_s.top(), from_range_s.size());

        auto from_range_q = range_to_queue(vals);
        std::cout << std::format("range_to_queue front={} size={}\n",
                                 from_range_q.front(), from_range_q.size());

        auto from_range_pq = range_to_pq(vals);
        std::cout << std::format("range_to_pq top(max)={}\n", from_range_pq.top());

        auto from_range_min = range_to_pq(vals, std::greater<int>{});
        std::cout << std::format("range_to_pq top(min)={}\n", from_range_min.top());
    }

    std::cout << "\n=== drain_to_vec (non-destructive via copy) ===\n";
    {
        std::priority_queue<int> copy_pq;
        for (int v : {3, 1, 4, 1, 5, 9, 2, 6}) copy_pq.push(v);
        const auto drained = drain_to_vec(copy_pq);
        std::cout << "Drained (sorted desc): ";
        for (int v : drained) std::cout << std::format("{} ", v);
        std::cout << '\n';
        std::cout << std::format("Original pq still has {} elements\n", copy_pq.size());
        assert(std::ranges::is_sorted(drained, std::greater<>{}));
    }

    std::cout << "\n=== k-largest via min-heap ===\n";
    {
        constexpr int k = 3;
        const std::vector<int> stream{5, 12, 3, 8, 20, 1, 15, 7};
        std::priority_queue<int, std::vector<int>, std::greater<>> min_k;
        for (int v : stream) {
            min_k.push(v);
            if (min_k.size() > k) min_k.pop();
        }
        const auto k_largest = drain_to_vec(min_k);
        std::cout << std::format("{}-largest: ", k);
        for (int v : k_largest) std::cout << std::format("{} ", v);
        std::cout << '\n';
        assert(k_largest.size() == k);
    }

    std::cout << "\n=== task scheduler via priority_queue ===\n";
    {
        struct Task {
            int         priority;
            std::string name;
            bool operator<(const Task& o) const noexcept {
                return priority < o.priority;
            }
        };
        std::priority_queue<Task> tasks;
        tasks.push({3, "Low priority task"});
        tasks.push({10, "Critical task"});
        tasks.push({7, "Medium task"});
        tasks.push({1, "Background task"});

        std::cout << "Processing tasks by priority:\n";
        while (!tasks.empty()) {
            const auto& [pri, name] = tasks.top();
            std::cout << std::format("  [{}] {}\n", pri, name);
            tasks.pop();
        }
    }

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
