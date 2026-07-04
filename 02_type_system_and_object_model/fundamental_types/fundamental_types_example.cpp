#include <array>
#include <bitset>
#include <cassert>
#include <cfloat>
#include <climits>
#include <iostream>
#include <limits>
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
			  << "  bool:      " << sizeof(bool) << " byte(s)\n";
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
			  << "  *(p+2)   = " << *(p + 2) << '\n';
}

enum class Day
{
	Monday,
	Tuesday,
	Wednesday
};

int main()
{
	std::cout << "\nAdvanced Fundamental Concepts:\n";

	// Signed-to-unsigned reinterpretation
	constexpr int neg = -1;
	const auto u_neg = static_cast<unsigned int>(neg);
	std::cout << "Signed -1 as unsigned: " << u_neg << '\n';

	// Floating-point precision
	const double fp_sum = 0.1 + 0.2;
	std::cout << "0.1 + 0.2 = " << fp_sum << '\n';
	std::cout << "float epsilon: " << std::numeric_limits<float>::epsilon() << '\n';

	// Pointer basics
	int value = 42;
	int* ptr = &value;
	std::cout << "Pointer: " << ptr << ", dereferenced: " << *ptr << '\n';

	int* null_ptr = nullptr;
	if (null_ptr == nullptr)
		std::cout << "Pointer is null\n";

	// Literal suffixes
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

	constexpr Day today = Day::Tuesday;
	std::cout << "\nEnum Day::Tuesday = " << static_cast<int>(today) << '\n';

	static_assert(sizeof(char) == 1);
	static_assert(sizeof(int) >= 4);
	static_assert(sizeof(float) == 4);
	assert(ptr == &value);
	assert(a == 10 && b == 5);

	std::cout << "\nAll assertions passed.\n";
	return 0;
}
