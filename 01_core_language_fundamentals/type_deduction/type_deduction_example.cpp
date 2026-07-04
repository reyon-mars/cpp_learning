#include <cassert>
#include <iostream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <vector>

template <typename T>
void print_type()
{
	std::cout << "type=" << typeid(T).name() << "\n";
}

template <typename T, typename U>
[[nodiscard]] auto multiply_generic(T a, U b) -> decltype(a * b)
{
	return a * b;
}

template <typename T, typename U>
[[nodiscard]] auto add_generic(T a, U b)
{
	return a + b;
}

template <typename T>
void forward_test(T&& param)
{
	if constexpr (std::is_lvalue_reference_v<T>)
		std::cout << "Lvalue\n";
	else
		std::cout << "Rvalue\n";
}

[[nodiscard]] decltype(auto) return_value()
{
	int x = 10;
	return x;
}

[[nodiscard]] decltype(auto) return_reference()
{
	static int x = 50;
	return (x);
}

template <typename T>
void check_integral()
{
	std::cout << "is_integral=" << std::boolalpha << std::is_integral_v<T> << "\n";
}

template <typename T>
void universal_ref_demo(T&& value)
{
	std::cout << "universal_ref value=" << value << "\n";
}

[[nodiscard]] constexpr auto square_auto(auto x) noexcept
{
	return x * x;
}

template <typename T>
void type_decay_demo(T)
{
	std::cout << "decayed type=" << typeid(T).name() << "\n";
}

template <typename T>
void print_vector_info(const std::vector<T>& vec)
{
	std::cout << "vector: size=" << vec.size() << " elem_type=" << typeid(T).name() << "\n";
}

static_assert(square_auto(6) == 36);
static_assert(square_auto(2.0) == 4.0);

int main()
{
	std::cout << "=== initializer_list deduction ===\n";
	auto list = {1, 2, 3};
	print_type<decltype(list)>();

	std::cout << "\n=== const stripping by auto ===\n";
	const int ci = 10;
	auto copy = ci;
	const auto copy2 = ci;
	std::cout << std::boolalpha << "copy is_const=" << std::is_const_v<decltype(copy)> << "\n"
			  << "copy2 is_const=" << std::is_const_v<decltype(copy2)> << "\n";

	std::cout << "\n=== trailing return type ===\n";
	auto mul = multiply_generic(2, 3.5);
	static_assert(std::is_same_v<decltype(mul), double>);
	std::cout << "multiply(2, 3.5)=" << mul << "\n";

	std::cout << "\n=== perfect forwarding ===\n";
	int a = 5;
	forward_test(a);
	forward_test(10);

	std::cout << "\n=== decltype(auto) value vs reference ===\n";
	auto val = return_value();
	std::cout << "return_value()=" << val << "\n";

	decltype(auto) ref = return_reference();
	assert(ref == 50);
	ref = 100;
	std::cout << "After ref=100, return_reference()=" << return_reference() << "\n";

	std::cout << "\n=== add_generic ===\n";
	auto sum = add_generic(10, 2.5);
	std::cout << "add(10, 2.5)=" << sum << "\n";

	std::cout << "\n=== check_integral ===\n";
	check_integral<int>();
	check_integral<double>();

	std::cout << "\n=== universal reference ===\n";
	universal_ref_demo(42);
	std::string text = "Hello";
	universal_ref_demo(text);

	std::cout << "\n=== constexpr auto (square_auto) ===\n";
	constexpr auto sq = square_auto(6);
	std::cout << "square_auto(6)=" << sq << "\n";

	std::cout << "\n=== type decay ===\n";
	const int value = 99;
	type_decay_demo(value);

	std::cout << "\n=== vector type info ===\n";
	const std::vector<std::string> names{"Alice", "Bob", "Charlie"};
	print_vector_info(names);

	std::cout << "\n=== decltype variable ===\n";
	decltype(sum) another = 55.5;
	std::cout << "decltype(sum) another=" << another << "\n";

	std::cout << "\nCompleted.\n";
	return 0;
}
