#include <cassert>
#include <iostream>
#include <memory>
#include <source_location>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <utility>
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
[[nodiscard]] consteval int consteval_factorial(int n)
{
	return n <= 1 ? 1 : n * consteval_factorial(n - 1);
}

constinit int program_epoch = 2026;

static_assert(square(5) == 25);
static_assert(cube(3) == 27);
static_assert(max_safe(5, 10) == 10);
static_assert(consteval_factorial(5) == 120);

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

[[nodiscard]] int call_sequence_number()
{
	static int calls = 0;
	return ++calls;
}

void thread_local_demo()
{
	auto worker = [](int id)
	{
		thread_local int hits = 0;
		for (int i = 0; i < 4; ++i) ++hits;
		std::cout << "thread " << id << " local hits=" << hits << "\n";
	};
	std::jthread t1{worker, 1};
	std::jthread t2{worker, 2};
}

void smart_pointer_example()
{
	auto ptr = std::make_unique<int>(200);
	assert(ptr != nullptr);
	std::cout << "unique_ptr value=" << *ptr << "\n";
}

void shared_and_weak_pointer_example()
{
	auto shared = std::make_shared<int>(300);
	const std::weak_ptr<int> weak = shared;
	std::cout << "shared_ptr value=" << *shared << ", use_count=" << shared.use_count() << "\n";
	std::cout << "weak_ptr expired? " << std::boolalpha << weak.expired() << "\n";
	shared.reset();
	std::cout << "weak_ptr expired after reset? " << weak.expired() << "\n";
	std::cout << "weak_ptr.lock() after reset is null? " << (weak.lock() == nullptr) << "\n";
}

void move_semantics_example()
{
	auto owner = std::make_unique<int>(555);
	const auto new_owner = std::move(owner);
	std::cout << "moved-from owner is null? " << std::boolalpha << (owner == nullptr) << "\n";
	std::cout << "new_owner value=" << *new_owner << "\n";
}

void custom_deleter_example()
{
	auto deleter = [](int* p)
	{
		std::cout << "custom deleter firing\n";
		delete p;
	};
	const std::unique_ptr<int, decltype(deleter)> guarded{new int{42}, deleter};
	std::cout << "guarded value=" << *guarded << "\n";
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
	explicit Resource(std::string name) : name_{std::move(name)}
	{
		std::cout << "Resource acquired: " << name_ << "\n";
	}
	~Resource()
	{
		if (!name_.empty())
			std::cout << "Resource released: " << name_ << "\n";
	}

	Resource(const Resource&) = delete;
	Resource& operator=(const Resource&) = delete;

	Resource(Resource&& other) noexcept : name_{std::exchange(other.name_, {})}
	{
	}
	Resource& operator=(Resource&& other) noexcept
	{
		if (this != &other)
		{
			name_ = std::exchange(other.name_, {});
		}
		return *this;
	}

	[[nodiscard]] std::string_view name() const noexcept { return name_; }

private:
	std::string name_;
};

void raii_demo()
{
	Resource res{"primary"};
	std::cout << "Using Resource safely\n";
}

void raii_move_demo()
{
	Resource original{"movable"};
	Resource moved_to = std::move(original);
	std::cout << "original.name() after move empty? " << std::boolalpha << original.name().empty() << "\n";
	std::cout << "moved_to.name()=" << moved_to.name() << "\n";
}

void raii_exception_safety_demo()
{
	try
	{
		Resource res{"exception-guarded"};
		std::cout << "About to throw while Resource is alive\n";
		throw std::runtime_error{"simulated failure"};
	}
	catch (const std::exception& e)
	{
		std::cout << "Caught: " << e.what() << " (Resource already released above)\n";
	}
}

class DebugHistory
{
public:
	enum class Level { Info, Warning, Error };

	void add(std::string_view msg, Level level = Level::Info)
	{
		entries_.push_back({std::string{msg}, level});
	}
	void print() const
	{
		std::cout << "Debug History:\n";
		for (const auto& [msg, level] : entries_)
			std::cout << "  [" << level_label(level) << "] " << msg << "\n";
	}
	void clear() noexcept
	{
		entries_.clear();
	}
	[[nodiscard]] std::size_t size() const noexcept
	{
		return entries_.size();
	}
	[[nodiscard]] auto begin() const noexcept { return entries_.begin(); }
	[[nodiscard]] auto end()   const noexcept { return entries_.end(); }

private:
	struct Entry
	{
		std::string message;
		Level level;
	};

	[[nodiscard]] static std::string_view level_label(Level level) noexcept
	{
		switch (level)
		{
			case Level::Info:    return "INFO";
			case Level::Warning: return "WARN";
			case Level::Error:   return "ERROR";
		}
		return "UNKNOWN";
	}

	std::vector<Entry> entries_;
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
			  << "max_safe(5,10)=" << max_safe(5, 10) << "\n"
			  << "consteval_factorial(5)=" << consteval_factorial(5) << "\n"
			  << "program_epoch=" << program_epoch << "\n";

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

	std::cout << "\n=== Static local storage duration ===\n";
	const int first_call = call_sequence_number();
	const int second_call = call_sequence_number();
	std::cout << "call_sequence_number: first=" << first_call << " second=" << second_call << "\n";

	std::cout << "\n=== thread_local storage duration ===\n";
	thread_local_demo();

	std::cout << "\n=== Smart pointer ===\n";
	smart_pointer_example();
	shared_and_weak_pointer_example();
	move_semantics_example();
	custom_deleter_example();

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
	raii_move_demo();
	raii_exception_safety_demo();

	std::cout << "\n=== debug_log (std::source_location) ===\n";
	debug_log("Testing debug log");
	debug_log("Second log entry");

	std::cout << "\n=== Debug history ===\n";
	DebugHistory hist;
	hist.add("Program initialized");
	hist.add("Storage demo completed");
	hist.add("Low disk space", DebugHistory::Level::Warning);
	hist.add("Translation unit demo completed");
	hist.print();
	std::cout << "Entries=" << hist.size() << "\n";
	std::cout << "Range-for over history:\n";
	for (const auto& entry : hist)
		std::cout << "  * " << entry.message << "\n";
	hist.clear();
	std::cout << "Entries after clear=" << hist.size() << "\n";

	std::cout << "\nCompleted successfully.\n";
	return 0;
}
