#include <array>
#include <bit>
#include <bitset>
#include <cassert>
#include <cfloat>
#include <climits>
#include <cmath>
#include <compare>
#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>
#include <optional>
#include <type_traits>

template <typename T>
void print_bits(T value)
{
	std::cout << "Bits: " << std::bitset<sizeof(T) * CHAR_BIT>(static_cast<std::make_unsigned_t<T>>(value)) << '\n';
}

void swap_values(int& a, int& b) noexcept
{
	int temp = a;
	a = b;
	b = temp;
}

void reference_demo()
{
	int x = 10;
	int& ref = x;
	ref = 20;
	std::cout << "x after reference modification: " << x << '\n';
}

void overflow_demo()
{
	unsigned char c = 255;
	std::cout << "Before overflow: " << static_cast<int>(c) << '\n';
	++c;
	std::cout << "After overflow:  " << static_cast<int>(c) << '\n';
}

void print_type_sizes()
{
	std::cout << "\nType sizes:\n"
			  << "  char:      " << sizeof(char) << " byte(s)\n"
			  << "  int:       " << sizeof(int) << " byte(s)\n"
			  << "  long long: " << sizeof(long long) << " byte(s)\n"
			  << "  double:    " << sizeof(double) << " byte(s)\n"
			  << "  long double: " << sizeof(long double) << " byte(s)\n"
			  << "  bool:      " << sizeof(bool) << " byte(s)\n"
			  << "  wchar_t:   " << sizeof(wchar_t) << " byte(s)\n"
			  << "  char8_t:   " << sizeof(char8_t) << " byte(s)\n"
			  << "  char16_t:  " << sizeof(char16_t) << " byte(s)\n"
			  << "  char32_t:  " << sizeof(char32_t) << " byte(s)\n";
}

void ascii_demo()
{
	constexpr char ch = 'A';
	std::cout << "\nASCII Demo:\n"
			  << "  Character:   " << ch << '\n'
			  << "  ASCII value: " << static_cast<int>(ch) << '\n'
			  << "  Next char:   " << static_cast<char>(ch + 1) << '\n';
}

template <typename T>
void show_type_info(const char* name)
{
	std::cout << "  " << name << " -> size: " << sizeof(T) << " bytes"
			  << ", signed: " << std::numeric_limits<T>::is_signed << ", min: " << +std::numeric_limits<T>::min()
			  << ", max: " << +std::numeric_limits<T>::max() << '\n';
}

void pointer_arithmetic_demo()
{
	constexpr std::array<int, 3> arr{10, 20, 30};
	const int* p = arr.data();
	std::cout << "\nPointer arithmetic:\n"
			  << "  *p       = " << *p << '\n'
			  << "  *(p+1)   = " << *(p + 1) << '\n'
			  << "  *(p+2)   = " << *(p + 2) << '\n'
			  << "  p[2]-p[0]= " << (&arr[2] - &arr[0]) << '\n';
}

[[nodiscard]] std::uint32_t float_bit_pattern(float f) noexcept
{
	return std::bit_cast<std::uint32_t>(f);
}

void float_representation_demo()
{
	constexpr float sample = 1.5F;
	std::cout << "\nFloating-point bit pattern:\n"
			  << "  value=" << sample << '\n'
			  << "  as bits=" << std::bitset<32>{float_bit_pattern(sample)} << '\n';
}

void literal_forms_demo()
{
	constexpr int hex_form = 0x2A;
	constexpr int octal_form = 052;
	constexpr int binary_form = 0b0010'1010;
	std::cout << "\nSame value, different literal forms:\n"
			  << "  hex 0x2A     = " << hex_form << '\n'
			  << "  octal 052    = " << octal_form << '\n'
			  << "  binary 0b101010 = " << binary_form << '\n';
}

void type_trait_checks_demo(bool is_day_enum)
{
	std::cout << "\nType trait checks:\n"
			  << "  is_arithmetic<int>    = " << std::boolalpha << std::is_arithmetic_v<int> << '\n'
			  << "  is_floating_point<int>= " << std::is_floating_point_v<int> << '\n'
			  << "  is_signed<unsigned>   = " << std::is_signed_v<unsigned int> << '\n'
			  << "  is_pointer<int*>      = " << std::is_pointer_v<int*> << '\n'
			  << "  is_enum<Day>          = " << is_day_enum << '\n';
}

enum class Day
{
	Monday,
	Tuesday,
	Wednesday
};

[[nodiscard]] constexpr auto to_underlying(Day d) noexcept
{
	return static_cast<std::underlying_type_t<Day>>(d);
}

void enum_demo()
{
	constexpr Day today = Day::Tuesday;
	constexpr Day tomorrow = Day::Wednesday;
	std::cout << "\nEnum Day::Tuesday = " << to_underlying(today) << '\n';
	std::cout << "today <=> tomorrow is ";
	if (const auto ordering = to_underlying(today) <=> to_underlying(tomorrow); ordering < 0)
		std::cout << "less\n";
	else if (ordering > 0)
		std::cout << "greater\n";
	else
		std::cout << "equal\n";
}

[[nodiscard]] std::optional<int> safe_divide(int numerator, int denominator) noexcept
{
	if (denominator == 0)
		return std::nullopt;
	return numerator / denominator;
}

void optional_demo()
{
	std::cout << "\nSafe division with std::optional:\n";
	if (const auto result = safe_divide(10, 2))
		std::cout << "  10 / 2 = " << *result << '\n';
	if (const auto result = safe_divide(10, 0))
		std::cout << "  10 / 0 = " << *result << '\n';
	else
		std::cout << "  10 / 0 -> no result (division by zero avoided)\n";
}

void structured_binding_demo()
{
	constexpr std::array<int, 3> coordinates{4, 5, 6};
	const auto [x, y, z] = coordinates;
	std::cout << "\nStructured bindings over std::array: x=" << x << " y=" << y << " z=" << z << '\n';
}

void midpoint_lerp_demo()
{
	std::cout << "\nOverflow-safe midpoint and lerp:\n"
			  << "  midpoint(INT_MAX-2, INT_MAX) = " << std::midpoint(INT_MAX - 2, INT_MAX) << '\n'
			  << "  lerp(0.0, 100.0, 0.25)       = " << std::lerp(0.0, 100.0, 0.25) << '\n';
}

int main()
{
	std::cout << "\nAdvanced Fundamental Concepts:\n";

	constexpr int neg = -1;
	const auto u_neg = static_cast<unsigned int>(neg);
	std::cout << "Signed -1 as unsigned: " << u_neg << '\n';

	const double fp_sum = 0.1 + 0.2;
	std::cout << "0.1 + 0.2 = " << fp_sum << '\n';
	std::cout << "float epsilon: " << std::numeric_limits<float>::epsilon() << '\n';

	int value = 42;
	int* ptr = &value;
	std::cout << "Pointer: " << ptr << ", dereferenced: " << *ptr << '\n';
	int* null_ptr = nullptr;
	if (null_ptr == nullptr)
		std::cout << "Pointer is null\n";

	constexpr auto big_num = 10'000'000'000LL;
	constexpr auto precise = 3.14L;
	std::cout << "big_num:  " << big_num << '\n';
	std::cout << "precise:  " << precise << '\n';

	print_type_sizes();

	std::cout << "\nBinary of 42:\n";
	print_bits(42);
	std::cout << "Binary of -1 (int):\n";
	print_bits(-1);

	reference_demo();
	overflow_demo();
	ascii_demo();
	float_representation_demo();
	literal_forms_demo();

	int a = 5, b = 10;
	std::cout << "\nBefore swap: " << a << ", " << b << '\n';
	swap_values(a, b);
	std::cout << "After swap:  " << a << ", " << b << '\n';

	std::cout << "\nInteger limits (std::numeric_limits):\n"
			  << "  INT_MAX: " << std::numeric_limits<int>::max() << '\n'
			  << "  INT_MIN: " << std::numeric_limits<int>::min() << '\n'
			  << "  DBL_MAX: " << std::numeric_limits<double>::max() << '\n';

	constexpr bool flag = true;
	std::cout << "\nBoolean: " << std::boolalpha << flag << '\n';

	std::cout << "\nType information:\n";
	show_type_info<char>("char");
	show_type_info<int>("int");
	show_type_info<unsigned int>("unsigned int");
	show_type_info<double>("double");

	pointer_arithmetic_demo();
	type_trait_checks_demo(std::is_enum_v<Day>);
	enum_demo();
	optional_demo();
	structured_binding_demo();
	midpoint_lerp_demo();

	static_assert(sizeof(char) == 1);
	static_assert(sizeof(int) >= 4);
	static_assert(sizeof(float) == 4);
	static_assert(std::is_enum_v<Day>);
	static_assert(to_underlying(Day::Monday) == 0);

	assert(ptr == &value);
	assert(a == 10 && b == 5);

	std::cout << "\nAll assertions passed.\n";
	return 0;
}
