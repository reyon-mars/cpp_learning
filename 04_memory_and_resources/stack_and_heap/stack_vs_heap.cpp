#include <iostream>
#include <memory>
#include <chrono>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>
#include <numeric>
#include <array>
#include <span>
#include <format>
#include <ranges>
#include <concepts>
#include <functional>
#include <optional>
#include <queue>

class LargeObject {
public:
    std::array<int, 1000> data{};

    LargeObject()                       { std::cout << "LargeObject constructed\n"; }
    LargeObject(const LargeObject&)     { std::cout << "LargeObject copied\n";      }
    LargeObject(LargeObject&&) noexcept { std::cout << "LargeObject moved\n";       }
    ~LargeObject()                      { std::cout << "LargeObject destroyed\n";   }

    LargeObject& operator=(const LargeObject&)     = default;
    LargeObject& operator=(LargeObject&&) noexcept = default;

    [[nodiscard]] bool operator==(const LargeObject& o) const noexcept {
        return data == o.data;
    }
};

void fill_object (LargeObject& obj, int value) noexcept { obj.data.fill(value); }
void clear_object(LargeObject& obj)            noexcept { obj.data.fill(0);     }

void copy_data(LargeObject& dest, const LargeObject& src) noexcept { dest.data = src.data; }

[[nodiscard]] long long sum_object(const LargeObject& obj) noexcept {
    return std::reduce(obj.data.cbegin(), obj.data.cend(), 0LL);
}

[[nodiscard]] double average_value(const LargeObject& obj) noexcept {
    return static_cast<double>(sum_object(obj)) / static_cast<double>(obj.data.size());
}

[[nodiscard]] int max_value(const LargeObject& obj) noexcept {
    return *std::ranges::max_element(obj.data);
}

[[nodiscard]] int min_value(const LargeObject& obj) noexcept {
    return *std::ranges::min_element(obj.data);
}

[[nodiscard]] bool compare_objects(const LargeObject& a, const LargeObject& b) noexcept {
    return a == b;
}

void print_preview(const LargeObject& obj, std::size_t count = 5) {
    const auto view = std::span{obj.data}.first(count);
    std::cout << "Preview: ";
    for (int v : view) std::cout << std::format("{} ", v);
    std::cout << '\n';
}

template <std::invocable<const LargeObject&> Fn>
void process_object(const LargeObject& obj, Fn&& fn) {
    std::invoke(std::forward<Fn>(fn), obj);
}

template <std::invocable<LargeObject&> Fn>
void transform_object(LargeObject& obj, Fn&& fn) {
    std::invoke(std::forward<Fn>(fn), obj);
}

using Clock = std::chrono::steady_clock;

template <std::invocable Fn>
[[nodiscard]] long long time_us(Fn&& fn) {
    const auto begin = Clock::now();
    std::invoke(std::forward<Fn>(fn));
    return std::chrono::duration_cast<std::chrono::microseconds>(
               Clock::now() - begin).count();
}

void measure_average(int iterations) {
    const auto stack_us = time_us([iterations] {
        for ([[maybe_unused]] int i : std::views::iota(0, iterations)) {
            LargeObject obj; (void)obj;
        }
    });
    std::cout << std::format("Avg stack: {} us\n", stack_us / iterations);

    const auto heap_us = time_us([iterations] {
        for ([[maybe_unused]] int i : std::views::iota(0, iterations))
            (void)std::make_unique<LargeObject>();
    });
    std::cout << std::format("Avg heap:  {} us\n", heap_us / iterations);
}

class ObjectPool {
    std::queue<std::unique_ptr<LargeObject>> pool_;
    std::size_t capacity_;

public:
    explicit ObjectPool(std::size_t cap) : capacity_(cap) {
        for ([[maybe_unused]] auto i : std::views::iota(std::size_t{0}, cap))
            pool_.push(std::make_unique<LargeObject>());
        std::cout << std::format("ObjectPool created with {} objects\n", cap);
    }

    [[nodiscard]] std::optional<std::unique_ptr<LargeObject>> acquire() {
        if (pool_.empty()) return std::nullopt;
        auto obj = std::move(pool_.front());
        pool_.pop();
        return obj;
    }

    void release(std::unique_ptr<LargeObject> obj) {
        if (pool_.size() < capacity_) {
            clear_object(*obj);
            pool_.push(std::move(obj));
        }
    }

    [[nodiscard]] std::size_t available() const noexcept { return pool_.size(); }
};

int main() {
    std::cout << std::format("sizeof(LargeObject)={} bytes\n", sizeof(LargeObject));

    std::cout << "\n=== Stack ===\n";
    {
        const auto us = time_us([] {
            LargeObject obj;
            fill_object(obj, 1);
            print_preview(obj);
            std::cout << std::format("Sum={}\n", sum_object(obj));
        });
        std::cout << std::format("Stack allocation: {} us\n", us);
    }

    std::cout << "\n=== Heap (raw) ===\n";
    {
        const auto us = time_us([] {
            auto* obj = new LargeObject{};
            fill_object(*obj, 2);
            print_preview(*obj);
            std::cout << std::format("Sum={}\n", sum_object(*obj));
            delete obj;
        });
        std::cout << std::format("Heap allocation: {} us\n", us);
    }

    std::cout << "\n=== Heap (unique_ptr) ===\n";
    {
        auto ptr = std::make_unique<LargeObject>();
        fill_object(*ptr, 3);
        print_preview(*ptr);
        std::cout << std::format("Sum={}\n", sum_object(*ptr));
        ptr.reset();
        std::cout << "unique_ptr reset\n";
    }

    std::cout << "\n=== Comparison & copy ===\n";
    {
        LargeObject a, b;
        fill_object(a, 5);
        copy_data(b, a);
        std::cout << std::format("Equal: {}\n", compare_objects(a, b));
    }

    std::cout << "\n=== shared_ptr & weak_ptr ===\n";
    {
        auto sp1 = std::make_shared<LargeObject>();
        std::weak_ptr<LargeObject> wp = sp1;
        auto sp2 = sp1;
        std::cout << std::format("use_count={}\n", sp1.use_count());
        sp2.reset();
        std::cout << std::format("use_count after sp2.reset={}\n", sp1.use_count());
        if (auto locked = wp.lock())
            std::cout << std::format("weak_ptr locked, use_count={}\n", locked.use_count());
    }

    std::cout << "\n=== Vector of unique_ptr ===\n";
    {
        std::vector<std::unique_ptr<LargeObject>> vec;
        vec.reserve(3);
        for (int i : std::views::iota(0, 3)) {
            vec.push_back(std::make_unique<LargeObject>());
            fill_object(*vec.back(), i);
        }
        for (const auto& obj : vec)
            std::cout << std::format("Sum={}\n", sum_object(*obj));

        auto sums = vec | std::views::transform([](const auto& o) { return sum_object(*o); });
        const long long total = std::reduce(sums.begin(), sums.end(), 0LL);
        std::cout << std::format("Total across all: {}\n", total);
    }

    std::cout << "\n=== Lambda processing ===\n";
    {
        LargeObject obj;
        fill_object(obj, 7);
        process_object(obj, [](const LargeObject& o) {
            std::cout << std::format("Lambda sum={}\n", sum_object(o));
        });
    }

    std::cout << "\n=== transform_object ===\n";
    {
        LargeObject obj;
        fill_object(obj, 4);
        transform_object(obj, [](LargeObject& o) {
            std::ranges::transform(o.data, o.data.begin(), [](int x) { return x * 2; });
        });
        print_preview(obj);
        std::cout << std::format("Sum after double={}\n", sum_object(obj));
    }

    std::cout << "\n=== Statistics ===\n";
    {
        LargeObject obj;
        fill_object(obj, 9);
        std::cout << std::format("Average={:.1f}\nMax={}\nMin={}\n",
                                 average_value(obj), max_value(obj), min_value(obj));
    }

    std::cout << "\n=== Clear ===\n";
    {
        LargeObject obj;
        fill_object(obj, 4);
        clear_object(obj);
        print_preview(obj);
    }

    std::cout << "\n=== Vector emplace ===\n";
    {
        std::vector<LargeObject> vec;
        vec.reserve(2);
        vec.emplace_back();
        vec.emplace_back();
        std::cout << std::format("size={}\n", vec.size());
    }

    std::cout << "\n=== Object Pool ===\n";
    {
        ObjectPool pool(3);
        std::cout << std::format("available={}\n", pool.available());

        auto obj1 = pool.acquire();
        assert(obj1.has_value());
        fill_object(**obj1, 42);
        std::cout << std::format("acquired, available={}\n", pool.available());
        print_preview(**obj1);

        pool.release(std::move(*obj1));
        std::cout << std::format("released, available={}\n", pool.available());
    }

    std::cout << "\n=== Average timing (1000 iterations) ===\n";
    measure_average(1000);

    std::cout << "\n=== Bulk timing (10000 iterations) ===\n";
    constexpr int N = 10'000;
    std::cout << std::format("Stack loop: {} ms\n", time_us([N] {
        for ([[maybe_unused]] int i : std::views::iota(0, N)) { LargeObject obj; (void)obj; }
    }) / 1000);
    std::cout << std::format("Heap loop:  {} ms\n", time_us([N] {
        for ([[maybe_unused]] int i : std::views::iota(0, N)) (void)std::make_unique<LargeObject>();
    }) / 1000);

    return 0;
}
