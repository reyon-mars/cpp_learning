#include <algorithm>
#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <optional>
#include <string_view>
#include <vector>
#include <ranges>
#include <cassert>
#include <numeric>

enum class Animal  { Cat, Dog, Bird, Fish };
enum class Vehicle { Car, Bike, Bus, Train };
enum class Food    { Pizza, Burger, Pasta };
enum class Color   { Red, Green, Blue };
enum class Season  { Spring, Summer, Autumn, Winter };

template<typename E>
struct enum_traits;

template<> struct enum_traits<Animal> {
    static constexpr std::array values{Animal::Cat, Animal::Dog, Animal::Bird, Animal::Fish};
    static constexpr std::string_view name(Animal v) noexcept {
        switch (v) {
            case Animal::Cat:  return "Cat";
            case Animal::Dog:  return "Dog";
            case Animal::Bird: return "Bird";
            case Animal::Fish: return "Fish";
        }
        return "Unknown";
    }
};

template<> struct enum_traits<Vehicle> {
    static constexpr std::array values{Vehicle::Car, Vehicle::Bike, Vehicle::Bus, Vehicle::Train};
    static constexpr std::string_view name(Vehicle v) noexcept {
        switch (v) {
            case Vehicle::Car:   return "Car";
            case Vehicle::Bike:  return "Bike";
            case Vehicle::Bus:   return "Bus";
            case Vehicle::Train: return "Train";
        }
        return "Unknown";
    }
};

template<> struct enum_traits<Food> {
    static constexpr std::array values{Food::Pizza, Food::Burger, Food::Pasta};
    static constexpr std::string_view name(Food v) noexcept {
        switch (v) {
            case Food::Pizza:  return "Pizza";
            case Food::Burger: return "Burger";
            case Food::Pasta:  return "Pasta";
        }
        return "Unknown";
    }
};

template<> struct enum_traits<Color> {
    static constexpr std::array values{Color::Red, Color::Green, Color::Blue};
    static constexpr std::string_view name(Color v) noexcept {
        switch (v) {
            case Color::Red:   return "Red";
            case Color::Green: return "Green";
            case Color::Blue:  return "Blue";
        }
        return "Unknown";
    }
};

template<> struct enum_traits<Season> {
    static constexpr std::array values{Season::Spring, Season::Summer, Season::Autumn, Season::Winter};
    static constexpr std::string_view name(Season v) noexcept {
        switch (v) {
            case Season::Spring: return "Spring";
            case Season::Summer: return "Summer";
            case Season::Autumn: return "Autumn";
            case Season::Winter: return "Winter";
        }
        return "Unknown";
    }
};

template<typename E>
concept HasEnumTraits = requires {
    enum_traits<E>::values;
    { enum_traits<E>::name(std::declval<E>()) } -> std::same_as<std::string_view>;
};

template<HasEnumTraits E>
[[nodiscard]] constexpr std::string_view enum_name(E value) noexcept {
    return enum_traits<E>::name(value);
}

template<HasEnumTraits E>
[[nodiscard]] constexpr auto enum_values() noexcept {
    return enum_traits<E>::values;
}

template<HasEnumTraits E>
[[nodiscard]] constexpr std::size_t enum_count() noexcept {
    return enum_traits<E>::values.size();
}

template<HasEnumTraits E>
[[nodiscard]] constexpr int to_int(E value) noexcept {
    return static_cast<int>(value);
}

template<HasEnumTraits E>
[[nodiscard]] constexpr bool is_valid(int value) noexcept {
    return std::ranges::any_of(enum_traits<E>::values,
                               [value](E e) { return to_int(e) == value; });
}

template<HasEnumTraits E>
[[nodiscard]] constexpr std::optional<E> from_int(int value) noexcept {
    if (!is_valid<E>(value)) return std::nullopt;
    return static_cast<E>(value);
}

template<HasEnumTraits E>
[[nodiscard]] std::optional<E> from_name(std::string_view name) noexcept {
    for (E v : enum_traits<E>::values)
        if (enum_traits<E>::name(v) == name) return v;
    return std::nullopt;
}

template<HasEnumTraits E>
[[nodiscard]] constexpr E next(E value) noexcept {
    const auto& vals = enum_traits<E>::values;
    for (std::size_t i = 0; i + 1 < vals.size(); ++i)
        if (vals[i] == value) return vals[i + 1];
    return vals[0];
}

template<HasEnumTraits E>
[[nodiscard]] constexpr E prev(E value) noexcept {
    const auto& vals = enum_traits<E>::values;
    for (std::size_t i = 1; i < vals.size(); ++i)
        if (vals[i] == value) return vals[i - 1];
    return vals.back();
}

template<HasEnumTraits E>
void print_all(std::string_view header = "") {
    if (!header.empty()) std::cout << header << "\n";
    for (E v : enum_values<E>())
        std::cout << std::format("  [{}] {}\n", to_int(v), enum_name(v));
}

template<HasEnumTraits E>
void print_enum_value(std::string_view label, E value) {
    std::cout << std::format("{}: {}\n", label, to_int(value));
}

template<HasEnumTraits E>
[[nodiscard]] std::vector<std::string_view> all_names() {
    std::vector<std::string_view> result;
    result.reserve(enum_count<E>());
    for (E v : enum_values<E>()) result.push_back(enum_name(v));
    return result;
}

void divider() { std::cout << "----------------------\n"; }

static_assert(enum_count<Animal>()  == 4);
static_assert(enum_count<Vehicle>() == 4);
static_assert(enum_count<Food>()    == 3);
static_assert(enum_count<Color>()   == 3);
static_assert(enum_count<Season>()  == 4);
static_assert(enum_name(Animal::Dog)    == "Dog");
static_assert(enum_name(Vehicle::Train) == "Train");
static_assert(to_int(Color::Green)      == 1);
static_assert(is_valid<Animal>(2));
static_assert(!is_valid<Animal>(99));

int main() {
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
        std::cout << std::format("from_int(1) -> {}\n", enum_name(*converted));
    divider();

    print_all<Animal>("All animals:");
    divider();
    print_all<Vehicle>("All vehicles:");
    divider();
    print_all<Color>("All colors:");
    divider();
    print_all<Season>("All seasons:");
    divider();

    std::cout << std::format("Dog == Cat? {}\n", a == Animal::Cat ? "Yes" : "No");
    std::cout << std::format("Car == Car? {}\n", v == Vehicle::Car ? "Yes" : "No");
    divider();

    std::cout << std::format("Animal::Dog  as int: {}\n", to_int(a));
    std::cout << std::format("Vehicle::Car as int: {}\n", to_int(v));
    std::cout << std::format("Food::Pizza  as int: {}\n", to_int(f));
    std::cout << std::format("Color::Green as int: {}\n", to_int(c));
    divider();

    std::cout << std::format("Animal count:  {}\n", enum_count<Animal>());
    std::cout << std::format("Vehicle count: {}\n", enum_count<Vehicle>());
    std::cout << std::format("Food count:    {}\n", enum_count<Food>());
    std::cout << std::format("Season count:  {}\n", enum_count<Season>());
    divider();

    std::cout << "Validation:\n";
    std::cout << std::format("is_valid<Animal>(2)=  {}\n", is_valid<Animal>(2)  ? "Yes" : "No");
    std::cout << std::format("is_valid<Animal>(10)= {}\n", is_valid<Animal>(10) ? "Yes" : "No");
    divider();

    std::cout << "from_name:\n";
    if (auto r = from_name<Vehicle>("Bus"))
        std::cout << std::format("  from_name('Bus') = {} (int {})\n", enum_name(*r), to_int(*r));
    if (!from_name<Vehicle>("Submarine"))
        std::cout << "  from_name('Submarine') = nullopt\n";
    divider();

    std::cout << "next / prev:\n";
    std::cout << std::format("  next(Animal::Dog)    = {}\n", enum_name(next(Animal::Dog)));
    std::cout << std::format("  next(Animal::Fish)   = {} (wraps)\n", enum_name(next(Animal::Fish)));
    std::cout << std::format("  prev(Animal::Dog)    = {}\n", enum_name(prev(Animal::Dog)));
    std::cout << std::format("  prev(Animal::Cat)    = {} (wraps)\n", enum_name(prev(Animal::Cat)));
    divider();

    std::cout << "all_names<Color>: ";
    for (auto n : all_names<Color>()) std::cout << n << " ";
    std::cout << "\n";
    divider();

    std::cout << "Generic integer values:\n";
    print_enum_value("Animal::Dog",  a);
    print_enum_value("Vehicle::Car", v);
    print_enum_value("Food::Pizza",  f);
    print_enum_value("Color::Green", c);
    divider();

    std::cout << "Summary:\n";
    std::cout << std::format("  Animals:  {}\n", enum_count<Animal>());
    std::cout << std::format("  Vehicles: {}\n", enum_count<Vehicle>());
    std::cout << std::format("  Foods:    {}\n", enum_count<Food>());
    std::cout << std::format("  Colors:   {}\n", enum_count<Color>());
    std::cout << std::format("  Seasons:  {}\n", enum_count<Season>());

    assert(enum_name(Animal::Cat) == "Cat");
    assert(from_int<Color>(1).value() == Color::Green);
    assert(!from_int<Color>(99).has_value());
    assert(from_name<Food>("Pasta").value() == Food::Pasta);
    assert(!from_name<Food>("Sushi").has_value());
    assert(next(Vehicle::Bike) == Vehicle::Bus);
    assert(prev(Vehicle::Car)  == Vehicle::Train);
    assert(all_names<Season>().size() == 4);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
