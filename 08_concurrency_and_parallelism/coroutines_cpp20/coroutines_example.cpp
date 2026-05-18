// Coroutines Exercise (C++20)
// stackless coroutines and co_await

#include <coroutine>
#include <exception>
#include <iostream>
#include <vector>      // 🔹 ADDED
#include <string>      // 🔹 ADDED

// Simplified coroutine example for C++20
class SimpleCoroutine
{
public:
	struct promise_type
	{
		int value;

		SimpleCoroutine get_return_object()
		{
			return SimpleCoroutine(std::coroutine_handle<promise_type>::from_promise(*this));
		}

		std::suspend_never initial_suspend()
		{
			return {};
		}

		std::suspend_never final_suspend() noexcept
		{
			return {};
		}

		void unhandled_exception()
		{
			std::terminate();
		}

		void return_value(int v)
		{
			value = v;
		}
	};

	using Handle = std::coroutine_handle<promise_type>;

	Handle handle;

	SimpleCoroutine(std::coroutine_handle<promise_type> h) : handle(h)
	{
	}

	~SimpleCoroutine()
	{
		if (handle)
			handle.destroy();
	}

	int get() const
	{
		return handle.promise().value;
	}

	void resume()
	{
		if (!handle.done())
			handle.resume();
	}
};

// Simple coroutine function
SimpleCoroutine compute_value()
{
	std::cout << "Coroutine started\n";
	co_return 42;
}

// Coroutine with suspension
SimpleCoroutine suspended_example()
{
	std::cout << "Before suspension\n";
	co_await std::suspend_always{};
	std::cout << "After resume\n";
	co_return 100;
}

// ---------------- SMALL ADDITIONS ----------------

// Multiple suspension example
SimpleCoroutine multi_suspend()
{
	std::cout << "Step 1\n";
	co_await std::suspend_always{};

	std::cout << "Step 2\n";
	co_await std::suspend_always{};

	std::cout << "Step 3\n";
	co_return 300;
}

// Coroutine demonstrating immediate execution
SimpleCoroutine quick_task()
{
	co_return 7 * 6;
}

// 🔹 NEW: Coroutine that resumes automatically
SimpleCoroutine auto_resume_task()
{
	std::cout << "Auto task start\n";
	co_return 555;
}

// 🔹 NEW: Helper to safely resume multiple times
void safe_resume(SimpleCoroutine& coro)
{
	if (!coro.handle.done()) {
		coro.resume();
	} else {
		std::cout << "Coroutine already finished\n";
	}
}

// ======================================================
// 🔥 EXTRA ADDITIONS
// ======================================================

// Coroutine with looped suspensions
SimpleCoroutine counting_coroutine(int limit)
{
	for (int i = 1; i <= limit; ++i)
	{
		std::cout << "Counter: " << i << "\n";
		co_await std::suspend_always{};
	}

	co_return limit;
}

// Coroutine processing vector values
SimpleCoroutine vector_sum_coroutine(const std::vector<int>& values)
{
	int total = 0;

	for (int v : values)
	{
		total += v;
		std::cout << "Adding: " << v << "\n";
		co_await std::suspend_always{};
	}

	co_return total;
}

// Coroutine returning calculated square
SimpleCoroutine square_coroutine(int value)
{
	std::cout << "Calculating square of " << value << "\n";
	co_return value * value;
}

// Coroutine chain demo
SimpleCoroutine chained_coroutine()
{
	std::cout << "Chain start\n";
	co_await std::suspend_always{};

	std::cout << "Chain continue\n";
	co_return 999;
}

// Helper: print coroutine status
void print_status(const std::string& name, const SimpleCoroutine& coro)
{
	std::cout << name << " done? "
			  << (coro.handle.done() ? "Yes" : "No")
			  << "\n";
}

// ======================================================

// ---------------- MAIN ----------------
int main()
{

	std::cout << "C++20 Coroutines support\n";

	auto coro = compute_value();
	std::cout << "Returned value: " << coro.get() << "\n";

	std::cout << "\nSuspension demo\n";

	auto coro2 = suspended_example();
	std::cout << "Resuming coroutine...\n";
	coro2.resume();

	std::cout << "Returned value: " << coro2.get() << "\n";


	// ---------------- ADDED USAGE ----------------

	std::cout << "\nMultiple suspension demo\n";
	auto coro3 = multi_suspend();

	std::cout << "Resume 1\n";
	coro3.resume();

	std::cout << "Resume 2\n";
	coro3.resume();

	std::cout << "Resume 3\n";
	coro3.resume();

	std::cout << "Final value: " << coro3.get() << "\n";

	// Quick coroutine
	auto coro4 = quick_task();
	std::cout << "Quick task result: " << coro4.get() << "\n";

	// 🔹 NEW: auto resume task
	auto coro5 = auto_resume_task();
	std::cout << "Auto task result: " << coro5.get() << "\n";

	// 🔹 NEW: safe resume demo
	std::cout << "\nSafe resume demo\n";
	safe_resume(coro3);  // already finished
	safe_resume(coro2);  // already finished

	// ======================================================
	// 🔥 EXTRA USAGE
	// ======================================================

	std::cout << "\n--- Counting Coroutine ---\n";

	auto counter_coro = counting_coroutine(3);

	print_status("counter_coro", counter_coro);

	counter_coro.resume();
	counter_coro.resume();
	counter_coro.resume();

	std::cout << "Counting coroutine result: "
			  << counter_coro.get() << "\n";

	print_status("counter_coro", counter_coro);

	// ------------------------------------------------------

	std::cout << "\n--- Vector Sum Coroutine ---\n";

	std::vector<int> nums = {1, 2, 3, 4};

	auto sum_coro = vector_sum_coroutine(nums);

	sum_coro.resume();
	sum_coro.resume();
	sum_coro.resume();
	sum_coro.resume();

	std::cout << "Vector sum result: "
			  << sum_coro.get() << "\n";

	// ------------------------------------------------------

	std::cout << "\n--- Square Coroutine ---\n";

	auto sq = square_coroutine(9);
	std::cout << "Square result: "
			  << sq.get() << "\n";

	// ------------------------------------------------------

	std::cout << "\n--- Chained Coroutine ---\n";

	auto chain = chained_coroutine();

	print_status("chain", chain);

	chain.resume();

	std::cout << "Chain result: "
			  << chain.get() << "\n";

	print_status("chain", chain);

	// ======================================================

	return 0;
}
