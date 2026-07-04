#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <optional>
#include <string_view>

enum class Animal
{
	Cat,
	Dog,
	Bird,
	Fish
};
enum class Vehicle
{
	Car,
	Bike,
	Bus,
	Train
};
enum class Food
{
	Pizza,
	Burger,
	Pasta
};
enum class Color
{
	Red,
	Green,
	Blue
};

template <typename E>
struct enum_traits;

template <>
struct enum_traits<Animal>
{
	static constexpr std::array values{Animal::Cat, Animal::Dog, Animal::Bird, Animal::Fish};
	static constexpr std::string_view name(Animal v) noexcept
	{
		switch (v)
		{
		case Animal::Cat:
			return "Cat";
		case Animal::Dog:
			return "Dog";
		case Animal::Bird:
			return "Bird";
		case Animal::Fish:
			return "Fish";
		}
		return "Unknown";
	}
};

template <>
struct enum_traits<Vehicle>
{
	static constexpr std::array values{Vehicle::Car, Vehicle::Bike, Vehicle::Bus, Vehicle::Train};
	static constexpr std::string_view name(Vehicle v) noexcept
	{
		switch (v)
		{
		case Vehicle::Car:
			return "Car";
		case Vehicle::Bike:
			return "Bike";
		case Vehicle::Bus:
			return "Bus";
		case Vehicle::Train:
			return "Train";
		}
		return "Unknown";
	}
};

template <>
struct enum_traits<Food>
{
	static constexpr std::array values{Food::Pizza, Food::Burger, Food::Pasta};
	static constexpr std::string_view name(Food v) noexcept
	{
		switch (v)
		{
		case Food::Pizza:
			return "Pizza";
		case Food::Burger:
			return "Burger";
		case Food::Pasta:
			return "Pasta";
		}
		return "Unknown";
	}
};

template <>
struct enum_traits<Color>
{
	static constexpr std::array values{Color::Red, Color::Green, Color::Blue};
	static constexpr std::string_view name(Color v) noexcept
	{
		switch (v)
		{
		case Color::Red:
			return "Red";
		case Color::Green:
			return "Green";
		case Color::Blue:
			return "Blue";
		}
		return "Unknown";
	}
};

template <typename E>
[[nodiscard]] constexpr std::string_view enum_name(E value) noexcept
{
	return enum_traits<E>::name(value);
}

template <typename E>
[[nodiscard]] constexpr auto enum_values() noexcept
{
	return enum_traits<E>::values;
}

template <typename E>
[[nodiscard]] constexpr std::size_t enum_count() noexcept
{
	return enum_traits<E>::values.size();
}

template <typename E>
[[nodiscard]] constexpr int to_int(E value) noexcept
{
	return static_cast<int>(value);
}

template <typename E>
[[nodiscard]] constexpr bool is_valid(int value) noexcept
{
	return std::ranges::any_of(enum_traits<E>::values,
							   [value](E e)
							   {
								   return to_int(e) == value;
							   });
}

template <typename E>
[[nodiscard]] constexpr std::optional<E> from_int(int value) noexcept
{
	if (!is_valid<E>(value))
		return std::nullopt;
	return static_cast<E>(value);
}

template <typename E>
void print_all()
{
	for (const auto v : enum_values<E>())
		std::cout << enum_name(v) << '\n';
}

template <typename E>
void print_enum_value(std::string_view label, E value)
{
	std::cout << std::format("{}: {}\n", label, to_int(value));
}

void divider()
{
	std::cout << "----------------------\n";
}

int main()
{
	constexpr Animal a = Animal::Dog;
	constexpr Vehicle v = Vehicle::Car;
	constexpr Food f = Food::Pizza;
	constexpr Color c = Color::Green;

	std::cout << "Basic Enum Usage:\n";
	std::cout << std::format("Animal: {}\n", enum_name(a));

	divider();

	std::cout << "\nAdvanced Enum Features:\n";
	std::cout << std::format("Vehicle name: {}\n", enum_name(v));
	std::cout << std::format("Food name:    {}\n", enum_name(f));
	std::cout << std::format("Color name:   {}\n", enum_name(c));

	if (const auto converted = from_int<Animal>(1))
		std::cout << std::format("Converted from int 1 -> {}\n", enum_name(*converted));

	divider();

	std::cout << "Iterating all animals:\n";
	print_all<Animal>();

	divider();

	std::cout << "All vehicles:\n";
	print_all<Vehicle>();

	divider();

	std::cout << "All colors:\n";
	print_all<Color>();

	divider();

	std::cout << std::format("Are Cat and Dog same? {}\n", a == Animal::Cat ? "Yes" : "No");
	std::cout << std::format("Are Car and Car same?  {}\n", v == Vehicle::Car ? "Yes" : "No");

	divider();

	std::cout << std::format("Animal (Dog) as int:   {}\n", to_int(a));
	std::cout << std::format("Vehicle (Car) as int:  {}\n", to_int(v));
	std::cout << std::format("Food (Pizza) as int:   {}\n", to_int(f));
	std::cout << std::format("Color (Green) as int:  {}\n", to_int(c));

	divider();

	std::cout << std::format("Total animals available:  {}\n", enum_count<Animal>());
	std::cout << std::format("Total vehicles available: {}\n", enum_count<Vehicle>());
	std::cout << std::format("Total foods available:    {}\n", enum_count<Food>());

	divider();

	std::cout << "Animal validation test:\n";
	std::cout << std::format("Is 2 valid?  {}\n", is_valid<Animal>(2) ? "Yes" : "No");
	std::cout << std::format("Is 10 valid? {}\n", is_valid<Animal>(10) ? "Yes" : "No");

	divider();

	std::cout << "Generic enum integer values:\n";
	print_enum_value("Animal::Dog", a);
	print_enum_value("Vehicle::Car", v);
	print_enum_value("Food::Pizza", f);
	print_enum_value("Color::Green", c);

	divider();

	std::cout << "Enum Statistics Summary:\n";
	std::cout << std::format("- Animals:  {}\n", enum_count<Animal>());
	std::cout << std::format("- Vehicles: {}\n", enum_count<Vehicle>());
	std::cout << std::format("- Foods:    {}\n", enum_count<Food>());
	std::cout << std::format("- Colors:   {}\n", enum_count<Color>());

	return 0;
}
