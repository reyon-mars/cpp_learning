#include <coroutine>
#include <cstdlib>
#include <exception>
#include <optional>
#include <print>
#include <vector>
#include <algorithm>   // 🔹 ADDED
#include <numeric>     // 🔹 ADDED
#include <memory>      // 🔹 ADDED

// ---------------- ORIGINAL CODE ----------------

template <typename T>
class generator
{
public:
	struct promise_type
	{
		std::optional<T> current_value;

		generator get_return_object()
		{
			return generator(handle::from_promise(*this));
		}

		std::suspend_always initial_suspend() noexcept
		{
			return {};
		}

		std::suspend_always final_suspend() noexcept
		{
			return {};
		}

		std::suspend_always yield_value(T value)
		{
			current_value = std::move(value);
			return {};
		}

		void return_void() noexcept
		{
		}

		void unhandled_exception()
		{
			std::terminate();
		}
	};

	using handle = std::coroutine_handle<promise_type>;

private:
	handle handle_;

public:
	explicit generator(handle h) : handle_(h)
	{
	}

	~generator()
	{
		if (handle_)
			handle_.destroy();
	}

	generator(const generator&) = delete;
	generator& operator=(const generator&) = delete;

	generator(generator&& other) noexcept : handle_(other.handle_)
	{
		other.handle_ = nullptr;
	}

	generator& operator=(generator&& other) noexcept
	{
		if (&other != this)
		{
			if (handle_)
				handle_.destroy();

			handle_ = other.handle_;
			other.handle_ = nullptr;
		}

		return *this;
	}

	bool resume()
	{
		if (!handle_ || handle_.done())
		{
			return false;
		}

		handle_.resume();
		return !handle_.done();
	}

	T value()
	{
		return handle_.promise().current_value.value();
	}
};

// ---------------- ORIGINAL GENERATOR ----------------

generator<int> counter()
{
	for (int i = 0; i < 10; i++)
	{
		co_yield i;
	}
}

// ---------------- SMALL ADDITIONS ----------------

// Another generator (squares)
generator<int> squares(int n)
{
	for (int i = 1; i <= n; ++i)
	{
		co_yield i * i;
	}
}

// 🔹 NEW: Fibonacci generator
generator<int> fibonacci(int n)
{
	int a = 0, b = 1;

	for (int i = 0; i < n; ++i)
	{
		co_yield a;

		int next = a + b;
		a = b;
		b = next;
	}
}

// Collect generator values into vector
template<typename T>
std::vector<T> collect(generator<T>& gen)
{
	std::vector<T> result;

	while (gen.resume())
	{
		result.push_back(gen.value());
	}

	return result;
}

// 🔹 NEW: Sum values from generator
template<typename T>
T sum_generator(generator<T>& gen)
{
	T sum{};

	while (gen.resume())
	{
		sum += gen.value();
	}

	return sum;
}

// ======================================================
// 🔥 EXTRA ADDITIONS
// ======================================================

// Reverse countdown generator
generator<int> countdown(int start)
{
	for (int i = start; i >= 0; --i)
	{
		co_yield i;
	}
}

// Even numbers generator
generator<int> even_numbers(int limit)
{
	for (int i = 0; i <= limit; i += 2)
	{
		co_yield i;
	}
}

// Find max value from generator
template<typename T>
T max_generator(generator<T>& gen)
{
	std::vector<T> values = collect(gen);

	return *std::max_element(values.begin(), values.end());
}

// Convert generator output into cumulative sums
template<typename T>
std::vector<T> cumulative_sum(generator<T>& gen)
{
	std::vector<T> result;
	T running{};

	while (gen.resume())
	{
		running += gen.value();
		result.push_back(running);
	}

	return result;
}

// Generator using dynamic memory
generator<int> pointer_values()
{
	auto ptr = std::make_unique<int>(50);

	co_yield *ptr;
	co_yield *ptr + 25;
	co_yield *ptr + 50;
}

// ======================================================
// MAIN
// ======================================================

auto main()->int
{
	auto gen = counter();

	gen.resume();
	std::println("The Current value is {}", gen.value());

	gen.resume();
	std::println("The new Current value is {}", gen.value());


	// ---------------- ADDED USAGE ----------------

	std::println("\nIterating remaining values:");

	while (gen.resume())
	{
		std::println("Next: {}", gen.value());
	}

	// Squares generator demo
	auto sq = squares(5);

	std::println("\nSquares:");

	while (sq.resume())
	{
		std::println("Square: {}", sq.value());
	}

	// Collect demo
	auto gen2 = counter();
	auto values = collect(gen2);

	std::println("\nCollected values:");

	for (auto v : values)
	{
		std::println("{}", v);
	}

	// 🔹 NEW: Fibonacci demo
	auto fib = fibonacci(8);

	std::println("\nFibonacci:");

	while (fib.resume())
	{
		std::println("Fib: {}", fib.value());
	}

	// 🔹 NEW: Sum generator demo
	auto gen3 = counter();
	auto total = sum_generator(gen3);

	std::println("\nSum of counter values: {}", total);

	// ======================================================
	// 🔥 EXTRA USAGE
	// ======================================================

	std::println("\n--- Extra Generator Tests ---");

	// Countdown demo
	auto down = countdown(5);

	std::println("\nCountdown:");

	while (down.resume())
	{
		std::println("{}", down.value());
	}

	// Even numbers demo
	auto evenGen = even_numbers(10);

	std::println("\nEven Numbers:");

	while (evenGen.resume())
	{
		std::println("Even: {}", evenGen.value());
	}

	// Max value demo
	auto gen4 = squares(6);
	auto maxVal = max_generator(gen4);

	std::println("\nMax square value: {}", maxVal);

	// Cumulative sum demo
	auto gen5 = counter();
	auto cumulative = cumulative_sum(gen5);

	std::println("\nCumulative sums:");

	for (auto v : cumulative)
	{
		std::println("{}", v);
	}

	// Pointer-based coroutine demo
	auto ptrGen = pointer_values();

	std::println("\nPointer values:");

	while (ptrGen.resume())
	{
		std::println("{}", ptrGen.value());
	}

	// ======================================================

	return EXIT_SUCCESS;
}
