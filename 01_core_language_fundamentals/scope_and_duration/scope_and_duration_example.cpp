#include <cassert>
#include <format>
#include <iostream>
#include <memory>
#include <source_location>
#include <span>
#include <string_view>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#define APP_VERSION "1.0"
#define DEBUG_MODE

#ifndef CONFIG_H
#define CONFIG_H
inline constexpr int max_users = 100;
#endif

#define SQUARE_MACRO(x) ((x) * (x))
#define BAD_SQUARE(x) x* x
#define STRINGIFY(x) #x
#define CONCAT(a, b) a##b
#define DEBUG_LOG(msg) std::cout << std::format("[DEBUG] {} ({}:{})\n", msg, __FILE__, __LINE__)
#define LOG_MANY(fmt, ...) std::cout << std::format(fmt, __VA_ARGS__)

#if defined(DEBUG_MODE)
inline constexpr std::string_view mode_status = "Debug Mode Active";
#else
inline constexpr std::string_view mode_status = "Release Mode";
#endif

void debug_message();

void print_version()
{
	std::cout << std::format("App Version: {}\n", APP_VERSION);
}

int build_number = 1;

namespace
{
	int internal_counter = 0;
	int internal_global = 999;
} // namespace

[[nodiscard]] constexpr int square(int x) noexcept
{
	return x * x;
}
[[nodiscard]] constexpr int max_safe(int a, int b) noexcept
{
	return a > b ? a : b;
}

[[nodiscard]] consteval int consteval_cube(int x)
{
	return x * x * x;
}

constinit int program_start_marker = 7;

void debug_message()
{
#ifdef DEBUG_MODE
	std::cout << "[DEBUG] Debug mode is ON\n";
#endif
}

extern int build_number;

void external_function()
{
	std::cout << "Simulated external function call\n";
}

void log_with_location(std::string_view message,
                        const std::source_location& loc = std::source_location::current())
{
	std::cout << std::format("[LOC] {} ({}:{}, in {})\n", message, loc.file_name(), loc.line(), loc.function_name());
}

void smart_pointer_example()
{
	const auto ptr = std::make_unique<int>(200);
	assert(ptr);
	std::cout << std::format("smart_pointer value: {}\n", *ptr);
}

void lifetime_demo()
{
	std::cout << "Entering lifetime_demo\n";
	const int temp = 123;
	std::cout << std::format("temp exists: {}\n", temp);
	std::cout << "Exiting lifetime_demo (temp destroyed)\n";
}

[[nodiscard]] std::unique_ptr<int> make_tracked(int value)
{
	return std::make_unique<int>(value);
}

void shared_pointer_example()
{
	const auto ptr1 = std::make_shared<int>(300);
	const auto ptr2 = ptr1;

	std::cout << std::format("shared_ptr value: {}\n", *ptr1);
	std::cout << std::format("Reference count: {}\n", ptr1.use_count());
}

void weak_pointer_example()
{
	auto shared = std::make_shared<int>(999);
	const std::weak_ptr<int> weak = shared;

	std::cout << std::format("weak_ptr expired? {}\n", weak.expired() ? "Yes" : "No");

	if (const auto locked = weak.lock())
		std::cout << std::format("weak_ptr locked value: {}\n", *locked);

	shared.reset();
	std::cout << std::format("weak_ptr expired after reset? {}\n", weak.expired() ? "Yes" : "No");
	std::cout << std::format("weak_ptr locked after expiry is null? {}\n", weak.lock() == nullptr ? "Yes" : "No");
}

struct tracked_resource : std::enable_shared_from_this<tracked_resource>
{
	int id;
	explicit tracked_resource(int i) noexcept : id{i} {}
	[[nodiscard]] std::shared_ptr<tracked_resource> self() { return shared_from_this(); }
};

void enable_shared_from_this_example()
{
	auto resource = std::make_shared<tracked_resource>(42);
	const auto alias = resource->self();
	std::cout << std::format("tracked_resource id={}, use_count={}\n", alias->id, resource.use_count());
}

void unique_ptr_array_with_custom_deleter()
{
	auto deleter = [](int* p)
	{
		std::cout << "custom deleter releasing array\n";
		delete[] p;
	};
	std::unique_ptr<int[], decltype(deleter)> arr{new int[3]{10, 20, 30}, deleter};
	for (int i = 0; i < 3; ++i)
		std::cout << std::format("arr[{}]={}\n", i, arr[i]);
}

void make_for_overwrite_example()
{
	auto uninitialized = std::make_unique_for_overwrite<int>();
	*uninitialized = 77;
	auto shared_uninitialized = std::make_shared_for_overwrite<int>();
	*shared_uninitialized = 88;
	std::cout << std::format("for_overwrite unique={} shared={}\n", *uninitialized, *shared_uninitialized);
}

[[nodiscard]] std::unique_ptr<int> exchange_ownership(std::unique_ptr<int>& source)
{
	return std::exchange(source, nullptr);
}

void memory_region_demo()
{
	const int stack_var = 10;
	const auto heap_var = std::make_unique<int>(20);

	std::cout << std::format("Stack variable: {}\n", stack_var);
	std::cout << std::format("Heap variable:  {}\n", *heap_var);
}

void move_demo()
{
	auto ptr1 = std::make_unique<int>(555);
	const auto ptr2 = std::move(ptr1);

	std::cout << std::format("Ownership transferred using move: {}\n", ptr1 == nullptr ? "Yes" : "No");
	std::cout << std::format("Moved value: {}\n", *ptr2);
}

[[nodiscard]] int call_counter()
{
	static int calls = 0;
	return ++calls;
}

void thread_local_demo()
{
	auto worker = [](int id)
	{
		thread_local int per_thread_hits = 0;
		for (int i = 0; i < 3; ++i) ++per_thread_hits;
		std::cout << std::format("thread {} sees per_thread_hits={}\n", id, per_thread_hits);
	};
	std::jthread t1{worker, 1};
	std::jthread t2{worker, 2};
}

template<typename T>
void describe_type(const T& value)
{
	if constexpr (std::is_pointer_v<T>)
		std::cout << std::format("pointer, points to value={}\n", *value);
	else if constexpr (std::is_integral_v<T>)
		std::cout << std::format("integral value={}\n", value);
	else
		std::cout << "some other type\n";
}

void mutable_span_demo(std::span<int> data)
{
	for (int& v : data) v *= 2;
	std::cout << "doubled: ";
	for (int v : data.first(data.size() > 2 ? 3 : data.size()))
		std::cout << v << ' ';
	std::cout << '\n';
}

int main()
{
	std::cout << "Compiled successfully\n";

	print_version();
	std::cout << std::format("Build number: {}\n", build_number);

	debug_message();

	++internal_counter;
	std::cout << std::format("Internal counter: {}\n", internal_counter);

	std::cout << std::format("Square of 5 (constexpr): {}\n", square(5));
	std::cout << std::format("Square of 5 (macro):     {}\n", SQUARE_MACRO(5));
	std::cout << std::format("Max users: {}\n", max_users);

	external_function();

	std::cout << "\nAdvanced Macro Features:\n";

	std::cout << std::format("max_safe(5,10): {}\n", max_safe(5, 10));
	DEBUG_LOG("Testing debug log");
	log_with_location("Testing source_location log");
	LOG_MANY("Variadic macro: {} + {} = {}\n", 2, 3, 2 + 3);
	std::cout << std::format("Mode: {}\n", mode_status);

	std::cout << std::format("BAD_SQUARE(2+3) [buggy]:  {}\n", BAD_SQUARE(2 + 3));
	std::cout << std::format("SQUARE_MACRO(2+3) [safe]: {}\n", SQUARE_MACRO(2 + 3));

	std::cout << std::format("STRINGIFY(TestMacro): {}\n", STRINGIFY(TestMacro));

	int CONCAT(my, Value) = 42;
	std::cout << std::format("Concatenated variable: {}\n", myValue);

	std::cout << std::format("consteval_cube(3): {}\n", consteval_cube(3));
	std::cout << std::format("constinit program_start_marker: {}\n", program_start_marker);

	std::cout << "\nAdvanced Storage Concepts:\n";

	std::cout << std::format("internal_global: {}\n", internal_global);
	const int first_call = call_counter();
	const int second_call = call_counter();
	const int third_call = call_counter();
	std::cout << std::format("call_counter first={}, second={}, third={}\n",
	                          first_call, second_call, third_call);

	smart_pointer_example();
	lifetime_demo();
	shared_pointer_example();
	weak_pointer_example();
	enable_shared_from_this_example();
	unique_ptr_array_with_custom_deleter();
	make_for_overwrite_example();
	memory_region_demo();
	move_demo();
	thread_local_demo();

	{
		auto original = std::make_unique<int>(321);
		auto moved = exchange_ownership(original);
		std::cout << std::format("exchange_ownership: original null={}, moved value={}\n",
		                          original == nullptr ? "Yes" : "No", *moved);
	}

	{
		auto tracked = make_tracked(500);
		assert(tracked != nullptr);
		std::cout << std::format("Tracked value: {}\n", *tracked);
	}
	std::cout << "Tracked pointer freed automatically at scope exit\n";

	const std::vector numbers{1, 2, 3, 4, 5};
	std::cout << "\nVector contents:\n";
	for (const int n : numbers)
		std::cout << n << ' ';
	std::cout << '\n';

	constexpr int compile_time_square = square(8);
	std::cout << std::format("Compile-time square: {}\n", compile_time_square);

	const int local_var = 100;
	std::cout << std::format("Address of local_var: {}\n", static_cast<const void*>(&local_var));

	const std::span<const int> view{numbers};
	std::cout << std::format("Span size: {}, first: {}\n", view.size(), view.front());

	std::vector mutable_numbers{1, 2, 3, 4};
	mutable_span_demo(mutable_numbers);

	int probe = 9;
	describe_type(probe);
	describe_type(&probe);

#undef DEBUG_MODE

	return 0;
}
