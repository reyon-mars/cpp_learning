#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <cassert>
#include <optional>
#include <initializer_list>
#include <compare>
#include <utility>
#include <concepts>

class SafeVector {
    std::vector<int> data_;

    [[nodiscard]] std::vector<int> copy_and_apply(auto op) const {
        auto temp = data_;
        op(temp);
        return temp;
    }

public:
    using iterator       = std::vector<int>::const_iterator;
    using const_iterator = std::vector<int>::const_iterator;

    SafeVector() = default;
    SafeVector(std::initializer_list<int> init) : data_(init) {}

    template<std::ranges::input_range R>
        requires std::convertible_to<std::ranges::range_value_t<R>, int>
    explicit SafeVector(R&& range) : data_(std::ranges::begin(range), std::ranges::end(range)) {}

    void add_element_strong(int value) {
        auto temp = copy_and_apply([&](auto& v) { v.push_back(value); });
        data_ = std::move(temp);
    }

    void add_element_basic(int value) {
        data_.push_back(value);
    }

    template<typename... Args>
        requires std::constructible_from<int, Args...>
    void emplace_back_strong(Args&&... args) {
        auto temp = copy_and_apply([&](auto& v) { v.emplace_back(std::forward<Args>(args)...); });
        data_ = std::move(temp);
    }

    void push_front_strong(int value) {
        auto temp = copy_and_apply([&](auto& v) { v.insert(v.begin(), value); });
        data_ = std::move(temp);
    }

    void insert_strong(std::size_t index, int value) {
        if (index > data_.size())
            throw std::out_of_range("insert_strong: index out of range");
        auto temp = copy_and_apply([&](auto& v) { v.insert(v.begin() + static_cast<std::ptrdiff_t>(index), value); });
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

    template<std::predicate<int> Pred>
    void erase_if_strong(Pred pred) {
        auto temp = copy_and_apply([&](auto& v) { std::erase_if(v, pred); });
        data_ = std::move(temp);
    }

    void reverse_strong() {
        auto temp = copy_and_apply([](auto& v) { std::ranges::reverse(v); });
        data_ = std::move(temp);
    }

    void sort_strong() {
        auto temp = copy_and_apply([](auto& v) { std::ranges::sort(v); });
        data_ = std::move(temp);
    }

    void unique_strong() {
        auto temp = copy_and_apply([](auto& v) {
            std::ranges::sort(v);
            const auto [first, last] = std::ranges::unique(v);
            v.erase(first, last);
        });
        data_ = std::move(temp);
    }

    void resize_strong(std::size_t n, int fill = 0) {
        auto temp = copy_and_apply([&](auto& v) { v.resize(n, fill); });
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
    void shrink_to_fit() { data_.shrink_to_fit(); }

    [[nodiscard]] int at(std::size_t index) const {
        if (index >= data_.size())
            throw std::out_of_range("at: index out of range");
        return data_[index];
    }

    [[nodiscard]] int operator[](std::size_t index) const noexcept { return data_[index]; }

    [[nodiscard]] std::optional<int> try_get(std::size_t index) const noexcept {
        if (index >= data_.size()) return std::nullopt;
        return data_[index];
    }

    [[nodiscard]] int front() const {
        if (data_.empty())
            throw std::runtime_error("front: vector is empty");
        return data_.front();
    }

    [[nodiscard]] int back() const {
        if (data_.empty())
            throw std::runtime_error("back: vector is empty");
        return data_.back();
    }

    [[nodiscard]] int sum()                    const noexcept { return std::accumulate(data_.begin(), data_.end(), 0); }
    [[nodiscard]] double average()              const {
        if (data_.empty())
            throw std::runtime_error("average: vector is empty");
        return static_cast<double>(sum()) / static_cast<double>(data_.size());
    }
    [[nodiscard]] std::optional<int> min()      const noexcept {
        if (data_.empty()) return std::nullopt;
        return std::ranges::min(data_);
    }
    [[nodiscard]] std::optional<int> max()      const noexcept {
        if (data_.empty()) return std::nullopt;
        return std::ranges::max(data_);
    }
    [[nodiscard]] bool contains(int value)     const noexcept { return std::ranges::contains(data_, value); }
    [[nodiscard]] std::size_t size()           const noexcept { return data_.size(); }
    [[nodiscard]] std::size_t capacity()       const noexcept { return data_.capacity(); }
    [[nodiscard]] bool empty()                 const noexcept { return data_.empty(); }
    [[nodiscard]] std::span<const int> view()  const noexcept { return data_; }

    [[nodiscard]] const_iterator begin() const noexcept { return data_.begin(); }
    [[nodiscard]] const_iterator end()   const noexcept { return data_.end(); }

    [[nodiscard]] bool operator==(const SafeVector& other) const noexcept { return data_ == other.data_; }
    [[nodiscard]] auto operator<=>(const SafeVector& other) const noexcept { return data_ <=> other.data_; }

    void print() const {
        for (int v : data_) std::cout << v << ' ';
        std::cout << '\n';
    }
};

std::ostream& operator<<(std::ostream& os, const SafeVector& sv) {
    os << '[';
    bool first = true;
    for (int v : sv) {
        if (!first) os << ", ";
        os << v;
        first = false;
    }
    return os << ']';
}

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

    std::cout << "\n--- New Features ---\n";

    SafeVector list{5, 3, 8, 1, 3, 9, 2};
    std::cout << "Initializer list: " << list << '\n';

    list.insert_strong(2, 100);
    std::cout << "After insert at 2: " << list << '\n';

    list.emplace_back_strong(42);
    std::cout << "After emplace_back: " << list << '\n';

    list.sort_strong();
    std::cout << "After sort: " << list << '\n';

    list.unique_strong();
    std::cout << "After unique: " << list << '\n';

    list.erase_if_strong([](int v) { return v > 50; });
    std::cout << "After erase_if(>50): " << list << '\n';

    std::cout << "Back: " << list.back() << '\n';
    std::cout << "Min: " << list.min().value_or(-1) << '\n';
    std::cout << "Max: " << list.max().value_or(-1) << '\n';
    std::cout << "Average: " << list.average() << '\n';
    std::cout << "operator[]: " << list[0] << '\n';

    list.resize_strong(3);
    std::cout << "After resize(3): " << list << '\n';

    std::cout << "\n--- Ranges & Iterators ---\n";
    const std::vector<int> source{1, 2, 3, 4, 5};
    SafeVector from_range(source);
    std::cout << "From range: " << from_range << '\n';

    auto even_sum = std::ranges::fold_left(
        from_range | std::views::filter([](int v) { return v % 2 == 0; }),
        0, std::plus<>{});
    std::cout << "Even sum via ranges: " << even_sum << '\n';

    std::cout << "Iterated: ";
    for (int v : from_range) std::cout << v << ' ';
    std::cout << '\n';

    SafeVector equal_check{1, 2, 3, 4, 5};
    assert(from_range == equal_check);
    assert(list != from_range);
    std::cout << "Comparison operators verified\n";

    std::cout << "\n--- Span view ---\n";
    for (int v : from_range.view()) std::cout << v << ' ';
    std::cout << '\n';

    sv.clear();
    std::cout << "After final clear: "; sv.print();
    std::cout << "Empty? " << (sv.empty() ? "Yes" : "No") << '\n';

    assert(sv.empty());
    std::cout << "\nAll assertions passed.\n";
    return 0;
}
