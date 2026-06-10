#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <cassert>
#include <optional>

class SafeVector {
    std::vector<int> data_;

    [[nodiscard]] std::vector<int> copy_and_apply(auto op) const {
        auto temp = data_;
        op(temp);
        return temp;
    }

public:
    void add_element_strong(int value) {
        auto temp = copy_and_apply([&](auto& v) { v.push_back(value); });
        data_ = std::move(temp);
    }

    void add_element_basic(int value) {
        data_.push_back(value);
    }

    void push_front_strong(int value) {
        auto temp = copy_and_apply([&](auto& v) { v.insert(v.begin(), value); });
        data_ = std::move(temp);
    }

    void replace_strong(std::size_t index, int value) {
        if (index >= data_.size())
            throw std::out_of_range("replace_strong: index out of range");
        auto temp = copy_and_apply([&](auto& v) { v[index] = value; });
        data_ = std::move(temp);
    }

    void remove_value_strong(int value) {
        auto temp = copy_and_apply([&](auto& v) {
            if (auto it = std::ranges::find(v, value); it != v.end())
                v.erase(it);
        });
        data_ = std::move(temp);
    }

    void reverse_strong() {
        auto temp = copy_and_apply([](auto& v) { std::ranges::reverse(v); });
        data_ = std::move(temp);
    }

    void pop_back() noexcept {
        if (!data_.empty()) data_.pop_back();
    }

    void swap(SafeVector& other) noexcept {
        data_.swap(other.data_);
    }

    void clear() noexcept { data_.clear(); }

    void reserve(std::size_t n) { data_.reserve(n); }

    [[nodiscard]] int at(std::size_t index) const {
        if (index >= data_.size())
            throw std::out_of_range("at: index out of range");
        return data_[index];
    }

    [[nodiscard]] std::optional<int> try_get(std::size_t index) const noexcept {
        if (index >= data_.size()) return std::nullopt;
        return data_[index];
    }

    [[nodiscard]] int front() const {
        if (data_.empty())
            throw std::runtime_error("front: vector is empty");
        return data_.front();
    }

    [[nodiscard]] int sum()                    const noexcept { return std::accumulate(data_.begin(), data_.end(), 0); }
    [[nodiscard]] bool contains(int value)     const noexcept { return std::ranges::contains(data_, value); }
    [[nodiscard]] std::size_t size()           const noexcept { return data_.size(); }
    [[nodiscard]] bool empty()                 const noexcept { return data_.empty(); }

    void print() const {
        for (int v : data_) std::cout << v << ' ';
        std::cout << '\n';
    }
};

int main() {
    SafeVector sv;

    std::cout << "--- Strong guarantee ---\n";
    sv.add_element_strong(1);
    sv.add_element_strong(2);
    sv.print();

    std::cout << "--- Basic guarantee ---\n";
    sv.add_element_basic(3);
    sv.add_element_basic(4);
    sv.print();

    std::cout << "Size: " << sv.size() << '\n';
    sv.reserve(10);

    sv.push_front_strong(0);
    sv.print();

    try {
        std::cout << "at(2): " << sv.at(2) << '\n';
        std::cout << sv.at(100) << '\n';
    } catch (const std::exception& e) {
        std::cout << "Caught: " << e.what() << '\n';
    }

    sv.clear();
    std::cout << "After clear: "; sv.print();

    SafeVector other;
    other.add_element_basic(99);
    sv.swap(other);
    std::cout << "After swap: "; sv.print();

    std::cout << "\n--- Extra Tests ---\n";
    sv.add_element_basic(10);
    sv.add_element_basic(20);
    sv.add_element_basic(30);
    sv.print();

    sv.pop_back();
    std::cout << "After pop_back: "; sv.print();

    sv.replace_strong(0, 999);
    std::cout << "After replace:  "; sv.print();

    if (auto val = sv.try_get(1)) {
        std::cout << "try_get(1): " << *val << '\n';
    } else {
        std::cout << "try_get failed\n";
    }

    std::cout << "Empty? " << (sv.empty() ? "Yes" : "No") << '\n';

    std::cout << "\n--- Advanced Tests ---\n";
    std::cout << "Contains 20? " << (sv.contains(20) ? "Yes" : "No") << '\n';
    std::cout << "Sum: " << sv.sum() << '\n';

    sv.reverse_strong();
    std::cout << "After reverse: "; sv.print();

    sv.remove_value_strong(20);
    std::cout << "After removing 20: "; sv.print();

    try {
        std::cout << "Front: " << sv.front() << '\n';
    } catch (const std::exception& e) {
        std::cout << "Front error: " << e.what() << '\n';
    }

    assert(!sv.empty());
    assert(sv.size() > 0);
    assert(sv.try_get(0).has_value());
    assert(!sv.try_get(999).has_value());

    sv.clear();
    std::cout << "After final clear: "; sv.print();
    std::cout << "Empty? " << (sv.empty() ? "Yes" : "No") << '\n';

    assert(sv.empty());
    std::cout << "\nAll assertions passed.\n";
    return 0;
}
