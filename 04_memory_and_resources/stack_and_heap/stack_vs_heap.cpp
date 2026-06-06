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

class LargeObject {
public:
    std::array<int, 1000> data{};

    LargeObject()                        { std::cout << "LargeObject constructed\n"; }
    LargeObject(const LargeObject&)      { std::cout << "LargeObject copied\n";      }
    LargeObject(LargeObject&&) noexcept  { std::cout << "LargeObject moved\n";       }
    ~LargeObject()                       { std::cout << "LargeObject destroyed\n";   }

    LargeObject& operator=(const LargeObject&)     = default;
    LargeObject& operator=(LargeObject&&) noexcept = default;
};

void fill_object(LargeObject& obj, int value) noexcept {
    obj.data.fill(value);
}

void clear_object(LargeObject& obj) noexcept {
    obj.data.fill(0);
}

[[nodiscard]] long long sum_object(const LargeObject& obj) noexcept {
    return std::accumulate(obj.data.cbegin(), obj.data.cend(), 0LL);
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
    return a.data == b.data;
}

void copy_data(LargeObject& dest, const LargeObject& src) noexcept {
    dest.data = src.data;
}

void print_preview(const LargeObject& obj, std::size_t count = 5) {
    std::cout << "Preview: ";
    const auto view = std::span{obj.data}.first(count);
    for (int v : view) { std::cout << v << " "; }
    std::cout << "\n";
}

template<typename Func>
void process_object(const LargeObject& obj, Func&& fn) {
    std::forward<Func>(fn)(obj);
}

using Clock = std::chrono::steady_clock;

template<typename Func>
[[nodiscard]] long long time_us(Func&& fn) {
    const auto begin = Clock::now();
    std::forward<Func>(fn)();
    return std::chrono::duration_cast<std::chrono::microseconds>(
               Clock::now() - begin).count();
}

void measure_average(int iterations) {
    const auto stack_us = time_us([iterations] {
        for (int i = 0; i < iterations; ++i) { LargeObject obj; (void)obj; }
    });
    std::cout << "Avg stack: " << stack_us / iterations << " us\n";

    const auto heap_us = time_us([iterations] {
        for (int i = 0; i < iterations; ++i) {
            auto p = std::make_unique<LargeObject>();
        }
    });
    std::cout << "Avg heap:  " << heap_us / iterations << " us\n";
}

int main() {
    std::cout << "sizeof(LargeObject)=" << sizeof(LargeObject) << " bytes\n";

    std::cout << "\n=== Stack ===\n";
    {
        const auto us = time_us([] {
            LargeObject obj;
            fill_object(obj, 1);
            print_preview(obj);
            std::cout << "Sum=" << sum_object(obj) << "\n";
        });
        std::cout << "Stack allocation: " << us << " us\n";
    }

    std::cout << "\n=== Heap (raw) ===\n";
    {
        const auto us = time_us([] {
            auto* obj = new LargeObject{};
            assert(obj != nullptr);
            fill_object(*obj, 2);
            print_preview(*obj);
            std::cout << "Sum=" << sum_object(*obj) << "\n";
            delete obj;
        });
        std::cout << "Heap allocation: " << us << " us\n";
    }

    std::cout << "\n=== Heap (unique_ptr) ===\n";
    {
        auto ptr = std::make_unique<LargeObject>();
        assert(ptr != nullptr);
        fill_object(*ptr, 3);
        print_preview(*ptr);
        std::cout << "Sum=" << sum_object(*ptr) << "\n";
        ptr.reset();
        std::cout << "unique_ptr reset\n";
    }

    std::cout << "\n=== Comparison & copy ===\n";
    {
        LargeObject a, b;
        fill_object(a, 5);
        copy_data(b, a);
        std::cout << "Equal: " << std::boolalpha << compare_objects(a, b) << "\n";
    }

    std::cout << "\n=== shared_ptr ===\n";
    {
        auto sp1 = std::make_shared<LargeObject>();
        auto sp2 = sp1;
        std::cout << "use_count=" << sp1.use_count() << "\n";
    }

    std::cout << "\n=== Vector of unique_ptr ===\n";
    {
        std::vector<std::unique_ptr<LargeObject>> vec;
        vec.reserve(3);
        for (int i = 0; i < 3; ++i) {
            vec.push_back(std::make_unique<LargeObject>());
            fill_object(*vec.back(), i);
        }
        for (const auto& obj : vec) {
            std::cout << "Sum=" << sum_object(*obj) << "\n";
        }
    }

    std::cout << "\n=== Lambda processing ===\n";
    {
        LargeObject obj;
        fill_object(obj, 7);
        process_object(obj, [](const LargeObject& o) {
            std::cout << "Lambda sum=" << sum_object(o) << "\n";
        });
    }

    std::cout << "\n=== Statistics ===\n";
    {
        LargeObject obj;
        fill_object(obj, 9);
        std::cout << "Average=" << average_value(obj) << "\n"
                  << "Max="     << max_value(obj)     << "\n"
                  << "Min="     << min_value(obj)     << "\n";
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
        std::cout << "size=" << vec.size() << "\n";
    }

    std::cout << "\n=== Average timing (1000 iterations) ===\n";
    measure_average(1000);

    std::cout << "\n=== Bulk timing (10000 iterations) ===\n";
    const int N = 10'000;
    std::cout << "Stack loop: " << time_us([N] {
        for (int i = 0; i < N; ++i) { LargeObject obj; (void)obj; }
    }) / 1000 << " ms\n";

    std::cout << "Heap loop:  " << time_us([N] {
        for (int i = 0; i < N; ++i) { auto p = std::make_unique<LargeObject>(); }
    }) / 1000 << " ms\n";

    return 0;
}
