#include <cassert>
#include <format>
#include <iostream>
#include <memory>
#include <span>
#include <string_view>
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

	shared.reset();
	std::cout << std::format("weak_ptr expired after reset? {}\n", weak.expired() ? "Yes" : "No");
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
	std::cout << std::format("Mode: {}\n", mode_status);

	std::cout << std::format("BAD_SQUARE(2+3) [buggy]:  {}\n", BAD_SQUARE(2 + 3));
	std::cout << std::format("SQUARE_MACRO(2+3) [safe]: {}\n", SQUARE_MACRO(2 + 3));

	std::cout << std::format("STRINGIFY(TestMacro): {}\n", STRINGIFY(TestMacro));

	int CONCAT(my, Value) = 42;
	std::cout << std::format("Concatenated variable: {}\n", myValue);

	std::cout << "\nAdvanced Storage Concepts:\n";

	std::cout << std::format("internal_global: {}\n", internal_global);

	smart_pointer_example();
	lifetime_demo();
	shared_pointer_example();
	weak_pointer_example();
	memory_region_demo();
	move_demo();

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

#undef DEBUG_MODE

	return 0;
}
