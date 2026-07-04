#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>
#include <string>
#include <string_view>
#include <vector>

inline constexpr std::string_view APP_VERSION = "1.0";
inline constexpr int MAX_USERS = 100;
inline constexpr bool DEBUG_MODE = true;
inline constexpr std::string_view MODE_STATUS = DEBUG_MODE ? "Debug Mode Active" : "Release Mode";

[[nodiscard]] constexpr int square(int x) noexcept
{
	return x * x;
}
[[nodiscard]] constexpr int cube(int x) noexcept
{
	return x * x * x;
}
[[nodiscard]] constexpr int max_safe(int a, int b) noexcept
{
	return a > b ? a : b;
}

static_assert(square(5) == 25);
static_assert(cube(3) == 27);
static_assert(max_safe(5, 10) == 10);

namespace
{

	int internal_counter = 0;
	int internal_global = 999;

	void hidden_function()
	{
		std::cout << "Hidden function (anonymous namespace)\n";
	}

} // namespace

int build_number = 1;
int external_counter = 5;

void print_version()
{
	std::cout << "App Version: " << APP_VERSION << "\n";
}

void debug_message()
{
	if constexpr (DEBUG_MODE)
		std::cout << "[DEBUG] Debug mode is ON\n";
}

void debug_log(std::string_view msg, const std::source_location loc = std::source_location::current())
{
	std::cout << "[DEBUG] " << msg << " (" << loc.file_name() << ":" << loc.line() << ")\n";
}

void external_function()
{
	std::cout << "Simulated external function call\n";
}

void modify_external()
{
	external_counter += 10;
	std::cout << "external_counter=" << external_counter << "\n";
}

inline void inline_helper()
{
	std::cout << "Inline helper\n";
}

void smart_pointer_example()
{
	auto ptr = std::make_unique<int>(200);
	assert(ptr != nullptr);
	std::cout << "unique_ptr value=" << *ptr << "\n";
}

void lifetime_demo()
{
	std::cout << "Entering lifetime_demo\n";
	int temp = 123;
	std::cout << "temp=" << temp << "\n";
	std::cout << "Exiting lifetime_demo\n";
}

class Resource
{
public:
	Resource()
	{
		std::cout << "Resource acquired\n";
	}
	~Resource()
	{
		std::cout << "Resource released\n";
	}
};

void raii_demo()
{
	Resource res;
	std::cout << "Using Resource safely\n";
}

class DebugHistory
{
public:
	void add(std::string_view msg)
	{
		entries_.emplace_back(msg);
	}
	void print() const
	{
		std::cout << "Debug History:\n";
		for (const auto& e : entries_)
			std::cout << "  - " << e << "\n";
	}
	[[nodiscard]] std::size_t size() const noexcept
	{
		return entries_.size();
	}

private:
	std::vector<std::string> entries_;
};

int main()
{
	std::cout << "=== Version / Build ===\n";
	print_version();
	std::cout << "build=" << build_number << "\n";
	debug_message();

	std::cout << "\n=== Constants ===\n";
	std::cout << "MAX_USERS=" << MAX_USERS << "\n"
			  << "Mode: " << MODE_STATUS << "\n";

	std::cout << "\n=== Constexpr functions ===\n";
	std::cout << "square(5)=" << square(5) << "\n"
			  << "cube(3)=" << cube(3) << "\n"
			  << "max_safe(5,10)=" << max_safe(5, 10) << "\n";

	std::cout << "\n=== Internal linkage ===\n";
	++internal_counter;
	std::cout << "internal_counter=" << internal_counter << "\n"
			  << "internal_global=" << internal_global << "\n";
	hidden_function();
	inline_helper();

	std::cout << "\n=== External linkage ===\n";
	external_function();
	std::cout << "external_counter=" << external_counter << "\n";
	modify_external();

	std::cout << "\n=== Smart pointer ===\n";
	smart_pointer_example();

	std::cout << "\n=== Lifetime ===\n";
	lifetime_demo();

	std::cout << "\n=== Raw pointer + null reset ===\n";
	{
		auto* raw = new int{500};
		assert(raw != nullptr);
		delete raw;
		raw = nullptr;
		std::cout << "raw after delete=" << std::boolalpha << (raw == nullptr) << "\n";
	}

	std::cout << "\n=== RAII ===\n";
	raii_demo();

	std::cout << "\n=== debug_log (std::source_location) ===\n";
	debug_log("Testing debug log");
	debug_log("Second log entry");

	std::cout << "\n=== Debug history ===\n";
	DebugHistory hist;
	hist.add("Program initialized");
	hist.add("Storage demo completed");
	hist.add("Translation unit demo completed");
	hist.print();
	std::cout << "Entries=" << hist.size() << "\n";

	std::cout << "\nCompleted successfully.\n";
	return 0;
}
