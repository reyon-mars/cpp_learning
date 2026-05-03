// Utilities and Helpers Exercise
// std::pair, std::tuple, std::optional, std::variant, std::any

#include <iostream>
#include <tuple>
#include <optional>
#include <variant>
#include <any>
#include <string>

int main() {
    // std::pair
    std::pair<int, std::string> p(42, "answer");
    std::cout << "Pair: " << p.first << ", " << p.second << "\n";
    
    // std::tuple
    auto t = std::make_tuple(1, "hello", 3.14);
    std::cout << "Tuple: " << std::get<0>(t) << ", " 
              << std::get<1>(t) << ", " << std::get<2>(t) << "\n";
    
    // std::optional
    std::optional<int> opt_val = 10;
    if (opt_val) {
        std::cout << "Optional value: " << opt_val.value() << "\n";
    }
    
    // std::variant
    std::variant<int, std::string> v = "hello";
    if (std::holds_alternative<std::string>(v)) {
        std::cout << "Variant string: " << std::get<std::string>(v) << "\n";
    }
    
    // std::any
    std::any a = 42;
    std::cout << "Any value: " << std::any_cast<int>(a) << "\n";

    // ---- Additional small examples ----

    // pair using make_pair
    auto p2 = std::make_pair(7, "days");
    std::cout << "Pair2: " << p2.first << ", " << p2.second << "\n";

    // tuple size
    std::cout << "Tuple size: " 
              << std::tuple_size<decltype(t)>::value << "\n";

    // optional reset
    opt_val.reset();
    std::cout << "Optional has value? "
              << (opt_val.has_value() ? "Yes" : "No") << "\n";

    // change variant value
    v = 100;
    if (std::holds_alternative<int>(v)) {
        std::cout << "Variant int: " << std::get<int>(v) << "\n";
    }

    // change any type
    a = std::string("stored string");
    if (a.type() == typeid(std::string)) {
        std::cout << "Any string: " 
                  << std::any_cast<std::string>(a) << "\n";
    }

    // -----------------------------------

    // ===== VERY SMALL NEW ADDITIONS =====

    // structured binding (pair)
    auto [num, text] = p;
    std::cout << "Structured binding: " << num << ", " << text << "\n";

    // tie unpacking (tuple)
    int i; std::string s; double d;
    std::tie(i, s, d) = t;
    std::cout << "Unpacked tuple: " << i << ", " << s << ", " << d << "\n";

    // optional with value_or
    std::optional<int> empty_opt;
    std::cout << "Optional value_or: "
              << empty_opt.value_or(999) << "\n";

    // variant index
    std::cout << "Variant index: " << v.index() << "\n";

    // safe any access
    if (a.has_value()) {
        std::cout << "Any has value\n";
    }

    // ---- EXTRA SMALL ADDITIONS ----

    // swap pairs
    std::pair<int, std::string> p3(1, "one");
    std::swap(p, p3);
    std::cout << "After swap, p: " << p.first << ", " << p.second << "\n";

    // get tuple element count via sizeof...
    std::cout << "Tuple element count (sizeof...): "
              << std::tuple_size<decltype(t)>::value << "\n";

    // optional emplace
    opt_val.emplace(55);
    std::cout << "Optional after emplace: "
              << opt_val.value() << "\n";

    // visit variant
    std::visit([](auto&& arg) {
        std::cout << "Visited variant value: " << arg << "\n";
    }, v);

    // reset any
    a.reset();
    std::cout << "Any after reset has value? "
              << (a.has_value() ? "Yes" : "No") << "\n";

    // --------------------------------

    return 0;
}
