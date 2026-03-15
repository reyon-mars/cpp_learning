#include <coroutine>
#include <exception>
#include <optional>

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
			current_value = value;
			return {};
		}
		void return_void()
		{
		}

		void unhandled_exception()
		{
			std::terminate();
		}
	};

	using handle = std::coroutine_handle<promise_type>;
	handle handle_;

	explicit generator(handle h) : handle_(h)
	{
	}
	~generator()
	{
		if (handle_)
			handle_.destroy();
	}
	generator(const generator&) = delete;
	generator(const generator&&) = delete;
};

generator<int> counter()
{
	for (int i = 0; i < 10; i++)
	{
		co_yield i;
	}
}