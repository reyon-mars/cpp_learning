#include <algorithm>
#include <array>
#include <cassert>
#include <concepts>
#include <cstddef>
#include <format>
#include <functional>
#include <iostream>
#include <optional>
#include <ranges>
#include <span>
#include <string_view>
#include <type_traits>

enum class Animal  { Cat, Dog, Bird, Fish };
enum class Vehicle { Car, Bike, Bus, Train };
enum class Food    { Pizza, Burger, Pasta };
enum class Color   { Red, Green, Blue };

template <typename E>
struct enum_traits;

template <>
struct enum_traits<Animal>
{
    static constexpr std::array values{Animal::Cat, Animal::Dog, Animal::Bird, Animal::Fish};
    static constexpr std::array<std::string_view, 4> descriptions{
        "A domesticated feline",
        "A loyal canine companion",
        "A feathered creature",
        "An aquatic vertebrate",
    };
    static constexpr std::string_view name(Animal v) noexcept
    {
        switch (v)
        {
        case Animal::Cat:  return "Cat";
        case Animal::Dog:  return "Dog";
        case Animal::Bird: return "Bird";
        case Animal::Fish: return "Fish";
        }
        return "Unknown";
    }
};

template <>
struct enum_traits<Vehicle>
{
    static constexpr std::array values{Vehicle::Car, Vehicle::Bike, Vehicle::Bus, Vehicle::Train};
    static constexpr std::array<std::string_view, 4> descriptions{
        "Four-wheeled motor vehicle",
        "Two-wheeled pedal vehicle",
        "Large public road transport",
        "Rail-based public transport",
    };
    static constexpr std::string_view name(Vehicle v) noexcept
    {
        switch (v)
        {
        case Vehicle::Car:   return "Car";
        case Vehicle::Bike:  return "Bike";
        case Vehicle::Bus:   return "Bus";
        case Vehicle::Train: return "Train";
        }
        return "Unknown";
    }
};

template <>
struct enum_traits<Food>
{
    static constexpr std::array values{Food::Pizza, Food::Burger, Food::Pasta};
    static constexpr std::array<std::string_view, 3> descriptions{
        "Italian flatbread with toppings",
        "Grilled patty in a bun",
        "Italian wheat noodle dish",
    };
    static constexpr std::string_view name(Food v) noexcept
    {
        switch (v)
        {
        case Food::Pizza:  return "Pizza";
        case Food::Burger: return "Burger";
        case Food::Pasta:  return "Pasta";
        }
        return "Unknown";
    }
};

template <>
struct enum_traits<Color>
{
    static constexpr std::array values{Color::Red, Color::Green, Color::Blue};
    static constexpr std::array<std::string_view, 3> descriptions{
        "Wavelength ~700 nm",
        "Wavelength ~530 nm",
        "Wavelength ~470 nm",
    };
    static constexpr std::string_view name(Color v) noexcept
    {
        switch (v)
        {
        case Color::Red:   return "Red";
        case Color::Green: return "Green";
        case Color::Blue:  return "Blue";
        }
        return "Unknown";
    }
};

template <typename E>
concept HasEnumTraits = requires {
    enum_traits<E>::values;
    enum_traits<E>::name(std::declval<E>());
};

template <HasEnumTraits E>
[[nodiscard]] constexpr std::string_view enum_name(E value) noexcept
{
    return enum_traits<E>::name(value);
}

template <HasEnumTraits E>
[[nodiscard]] constexpr auto enum_values() noexcept
{
    return enum_traits<E>::values;
}

template <HasEnumTraits E>
[[nodiscard]] constexpr std::size_t enum_count() noexcept
{
    return enum_traits<E>::values.size();
}

template <HasEnumTraits E>
[[nodiscard]] constexpr int to_int(E value) noexcept
{
    return static_cast<int>(value);
}

template <HasEnumTraits E>
[[nodiscard]] constexpr bool is_valid(int value) noexcept
{
    return std::ranges::any_of(enum_traits<E>::values,
        [value](E e) { return to_int(e) == value; });
}

template <HasEnumTraits E>
[[nodiscard]] constexpr std::optional<E> from_int(int value) noexcept
{
    if (!is_valid<E>(value)) return std::nullopt;
    return static_cast<E>(value);
}

template <HasEnumTraits E>
[[nodiscard]] std::optional<E> find_by_name(std::string_view name) noexcept
{
    const auto it = std::ranges::find_if(enum_traits<E>::values,
        [name](E e) { return enum_traits<E>::name(e) == name; });
    if (it == enum_traits<E>::values.end()) return std::nullopt;
    return *it;
}

template <HasEnumTraits E>
[[nodiscard]] constexpr std::string_view enum_description(E value) noexcept
{
    const auto idx = static_cast<std::size_t>(to_int(value));
    const auto& descs = enum_traits<E>::descriptions;
    return (idx < descs.size()) ? descs[idx] : "No description";
}

template <HasEnumTraits E>
void print_all()
{
    for (const auto v : enum_values<E>())
        std::cout << enum_name(v) << '\n';
}

template <HasEnumTraits E>
void print_all_with_descriptions()
{
    for (const auto v : enum_values<E>())
        std::cout << std::format("  {:10} - {}\n", enum_name(v), enum_description(v));
}

template <HasEnumTraits E, std::invocable<E> Func>
void enum_for_each(Func&& fn)
{
    for (const auto v : enum_values<E>())
        std::forward<Func>(fn)(v);
}

template <HasEnumTraits E>
void print_enum_value(std::string_view label, E value)
{
    std::cout << std::format("{}: {}\n", label, to_int(value));
}

template <HasEnumTraits E>
[[nodiscard]] constexpr std::optional<E> next_enum(E value) noexcept
{
    const auto idx = static_cast<std::size_t>(to_int(value));
    const auto& vals = enum_traits<E>::values;
    return (idx + 1 < vals.size()) ? std::optional{vals[idx + 1]} : std::nullopt;
}

template <HasEnumTraits E>
[[nodiscard]] constexpr std::optional<E> prev_enum(E value) noexcept
{
    const auto idx = static_cast<std::size_t>(to_int(value));
    const auto& vals = enum_traits<E>::values;
    return (idx > 0) ? std::optional{vals[idx - 1]} : std::nullopt;
}

void divider() { std::cout << "----------------------\n"; }

static_assert(enum_count<Animal>()  == 4);
static_assert(enum_count<Vehicle>() == 4);
static_assert(enum_count<Food>()    == 3);
static_assert(enum_count<Color>()   == 3);
static_assert(is_valid<Animal>(0));
static_assert(!is_valid<Animal>(99));
static_assert(to_int(Animal::Cat)  == 0);
static_assert(to_int(Animal::Fish) == 3);

int main()
{
    constexpr Animal  a = Animal::Dog;
    constexpr Vehicle v = Vehicle::Car;
    constexpr Food    f = Food::Pizza;
    constexpr Color   c = Color::Green;

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
    print_enum_value("Animal::Dog",   a);
    print_enum_value("Vehicle::Car",  v);
    print_enum_value("Food::Pizza",   f);
    print_enum_value("Color::Green",  c);

    divider();

    std::cout << "Enum Statistics Summary:\n";
    std::cout << std::format("- Animals:  {}\n", enum_count<Animal>());
    std::cout << std::format("- Vehicles: {}\n", enum_count<Vehicle>());
    std::cout << std::format("- Foods:    {}\n", enum_count<Food>());
    std::cout << std::format("- Colors:   {}\n", enum_count<Color>());

    divider();

    std::cout << "\nDescriptions:\n";
    print_all_with_descriptions<Animal>();
    divider();
    print_all_with_descriptions<Vehicle>();
    divider();
    print_all_with_descriptions<Food>();

    std::cout << "\nfind_by_name:\n";
    if (const auto found = find_by_name<Animal>("Bird"))
        std::cout << std::format("Found: {} (int={})\n", enum_name(*found), to_int(*found));
    if (!find_by_name<Animal>("Elephant"))
        std::cout << "\"Elephant\" not found (correct)\n";

    std::cout << "\nenum_for_each (colors):\n";
    enum_for_each<Color>([](Color col) {
        std::cout << std::format("  {} -> {}\n", enum_name(col), enum_description(col));
    });

    std::cout << "\nnext/prev enum:\n";
    if (const auto next = next_enum(Animal::Dog))
        std::cout << std::format("After Dog: {}\n", enum_name(*next));
    if (const auto prev = prev_enum(Animal::Dog))
        std::cout << std::format("Before Dog: {}\n", enum_name(*prev));
    std::cout << std::format("After Fish (none): {}\n",
                             next_enum(Animal::Fish).has_value() ? "exists" : "nullopt");

    std::cout << "\nAll assertions passed (static_asserts at compile time).\n";

    assert(find_by_name<Animal>("Cat").value() == Animal::Cat);
    assert(!find_by_name<Vehicle>("Submarine").has_value());
    assert(next_enum(Color::Red).value() == Color::Green);
    assert(!prev_enum(Color::Red).has_value());
    assert(enum_description(Food::Burger) == "Grilled patty in a bun");

    std::cout << "All runtime assertions passed.\n";
    return 0;
}
