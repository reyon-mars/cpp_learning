#include <coroutine>
#include <cstdlib>
#include <exception>
#include <optional>
#include <print>
#include <vector>

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

// ---------------- MAIN ----------------

auto main()->int
{
	auto gen = counter();

	gen.resume();
	std::println("The Current value is {}", gen.value() );

	gen.resume();
	std::println( "The new Current value is {}", gen.value());


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

	return EXIT_SUCCESS;
}
