#include <iostream>
#include <concepts>
#include <ranges>
#include <vector>
#include <string>
#include <string_view>
#include <numeric>
#include <algorithm>
#include <format>
#include <coroutine>
#include <iterator>
#include <cassert>

template <typename T>
concept Printable = requires(std::ostream& os, T t) { os << t; };

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <typename T>
concept SizedRange = std::ranges::sized_range<T>;

template <typename T>
concept SortableRange =
    std::ranges::random_access_range<T> &&
    std::sortable<std::ranges::iterator_t<T>>;

template <Printable T>
void print(const T& t) { std::cout << t << '\n'; }

template <Number T>
[[nodiscard]] constexpr T add(T a, T b) noexcept { return a + b; }

template <Number T>
[[nodiscard]] constexpr T square(T x) noexcept { return x * x; }

template <SizedRange R>
    requires std::integral<std::ranges::range_value_t<R>>
[[nodiscard]] double average(const R& r) noexcept {
    const auto n = static_cast<double>(std::ranges::size(r));
    return n == 0.0 ? 0.0
                    : std::reduce(std::ranges::begin(r),
                                  std::ranges::end(r), 0.0) / n;
}

template <std::ranges::input_range R>
void print_range(std::string_view label, R&& r) {
    std::cout << label << ": ";
    for (const auto& v : r) std::cout << v << ' ';
    std::cout << '\n';
}

template <typename T>
struct Generator {
    struct promise_type {
        T current_value{};

        Generator           get_return_object() { return Generator{this}; }
        std::suspend_always initial_suspend()   noexcept { return {}; }
        std::suspend_always final_suspend()     noexcept { return {}; }
        std::suspend_always yield_value(T v)    noexcept {
            current_value = v;
            return {};
        }
        void return_void()  noexcept {}
        void unhandled_exception() { std::terminate(); }
    };

    struct iterator {
        using iterator_category = std::input_iterator_tag;
        using value_type        = T;
        using difference_type   = std::ptrdiff_t;

        std::coroutine_handle<promise_type> handle{};

        iterator& operator++() { handle.resume(); return *this; }
        iterator  operator++(int) { auto copy = *this; ++*this; return copy; }
        const T&  operator*()  const { return handle.promise().current_value; }
        bool      operator==(std::default_sentinel_t) const { return handle.done(); }
    };

    iterator               begin() { handle_.resume(); return {handle_}; }
    std::default_sentinel_t end()  { return {}; }

    ~Generator() { if (handle_) handle_.destroy(); }

    Generator(const Generator&)            = delete;
    Generator& operator=(const Generator&) = delete;

private:
    using Handle = std::coroutine_handle<promise_type>;
    explicit Generator(promise_type* p) : handle_{Handle::from_promise(*p)} {}
    Handle handle_;
};

Generator<int> iota_gen(int start, int stop, int step = 1) {
    for (int i = start; i < stop; i += step)
        co_yield i;
}

Generator<int> fibonacci() {
    int a = 0, b = 1;
    while (true) {
        co_yield a;
        a = std::exchange(b, a + b);
    }
}

int main() {
    print(42);
    print(std::string_view{"hello"});
    print(3.14);

    std::cout << std::format("Add ints:    {}\n", add(5, 3));
    std::cout << std::format("Add doubles: {}\n", add(2.5, 4.1));

    static_assert(Number<int>);
    static_assert(Number<double>);
    static_assert(!Number<std::string>);

    const std::vector vec {1, 2, 3, 4, 5};

    print_range("Evens",       vec | std::views::filter([](int x){ return x % 2 == 0; }));
    print_range("Squares",     vec | std::views::transform(square<int>));
    print_range("First three", vec | std::views::take(3));
    print_range("After two",   vec | std::views::drop(2));
    print_range("Reversed",    vec | std::views::reverse);

    print_range("Even squares",
        vec | std::views::filter([](int x){ return x % 2 == 0; })
            | std::views::transform(square<int>));

    print_range("Take-drop slice",
        vec | std::views::drop(1) | std::views::take(3));

    static_assert(std::ranges::sized_range<std::vector<int>>);
    static_assert(SortableRange<std::vector<int>>);

    std::cout << "----------------------\n";

    std::cout << std::format("Sum:         {}\n",
                             std::reduce(vec.begin(), vec.end(), 0));
    std::cout << std::format("Average:     {}\n", average(vec));
    std::cout << std::format("Even count:  {}\n",
                             std::ranges::count_if(vec, [](int x){ return x % 2 == 0; }));
    std::cout << std::format("Contains 3?  {}\n",
                             std::ranges::contains(vec, 3) ? "Yes" : "No");
    std::cout << std::format("All positive?{}\n",
                             std::ranges::all_of(vec, [](int x){ return x > 0; }) ? "Yes" : "No");
    std::cout << std::format("Is sorted?   {}\n",
                             std::ranges::is_sorted(vec) ? "Yes" : "No");

    const auto [min_it, max_it] = std::ranges::minmax_element(vec);
    std::cout << std::format("Min: {}, Max: {}\n", *min_it, *max_it);

    std::cout << "----------------------\n";

    std::cout << "iota_gen(0, 10, 2): ";
    for (const int v : iota_gen(0, 10, 2)) std::cout << v << ' ';
    std::cout << '\n';

    std::cout << "fibonacci (first 10): ";
    int count = 0;
    for (const int v : fibonacci()) {
        std::cout << v << ' ';
        if (++count == 10) break;
    }
    std::cout << '\n';

    std::cout << "iota_gen sum: ";
    int gen_sum = 0;
    for (const int v : iota_gen(1, 6)) gen_sum += v;
    std::cout << std::format("{}\n", gen_sum);

    return 0;
}
