#include <iostream>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

void print_map(const std::map<std::string, int>& m) {
    for (const auto& [name, age] : m)
        std::cout << "  " << name << "=" << age << "\n";
}

void print_set(const auto& s) {
    for (const auto& v : s) std::cout << v << " ";
    std::cout << "\n";
}

int main() {
    std::cout << "=== std::map ===\n";
    std::map<std::string, int> ages{
        {"Alice", 30},
        {"Bob",   25},
    };
    std::cout << "Alice=" << ages.at("Alice") << "\n";

    ages.emplace("Charlie", 28);
    ages.emplace("David",   40);

    std::cout << "All entries:\n";
    print_map(ages);

    if (auto it = ages.find("Bob"); it != ages.end())
        std::cout << "Found Bob=" << it->second << "\n";

    std::cout << "contains(Alice)=" << std::boolalpha << ages.contains("Alice") << "\n";

    ages.at("Alice") += 1;
    std::cout << "Updated Alice=" << ages.at("Alice") << "\n";

    ages.erase("David");
    std::cout << "After erasing David:\n";
    print_map(ages);

    std::cout << "size=" << ages.size() << "\n";

    std::cout << "\n=== std::set ===\n";
    std::set<int> unique_values{5, 2, 8, 2, 5, 1};
    std::cout << "Unique: "; print_set(unique_values);

    std::cout << "contains(5)=" << unique_values.contains(5) << "\n";

    unique_values.erase(2);
    std::cout << "After erase(2): "; print_set(unique_values);

    auto lb = unique_values.lower_bound(5);
    auto ub = unique_values.upper_bound(5);
    std::cout << "lower_bound(5)=" << (lb != unique_values.end() ? std::to_string(*lb) : "end") << "\n"
              << "upper_bound(5)=" << (ub != unique_values.end() ? std::to_string(*ub) : "end") << "\n";

    std::set<int> other_set{100, 200, 300};
    unique_values.swap(other_set);
    std::cout << "After swap: "; print_set(unique_values);
    unique_values.swap(other_set);
    std::cout << "Restored:   "; print_set(unique_values);

    std::cout << "size=" << unique_values.size() << "\n";

    std::cout << "\n=== std::multimap ===\n";
    std::multimap<std::string, int> scores{
        {"Alice", 90},
        {"Alice", 85},
        {"Bob",   88},
    };
    for (const auto& [name, score] : scores)
        std::cout << "  " << name << "=" << score << "\n";

    auto [first, last] = scores.equal_range("Alice");
    std::cout << "Alice scores: ";
    for (auto it = first; it != last; ++it) std::cout << it->second << " ";
    std::cout << "\n";

    std::cout << "count(Alice)=" << scores.count("Alice") << "\n";

    std::cout << "\n=== std::multiset ===\n";
    std::multiset<int> numbers{5, 2, 8, 2, 5, 1};
    std::cout << "Values: "; print_set(numbers);
    std::cout << "count(5)=" << numbers.count(5) << "\n";

    numbers.clear();
    std::cout << "After clear: empty=" << numbers.empty() << " size=" << numbers.size() << "\n";

    numbers.insert(10);
    numbers.insert(10);
    numbers.insert(20);
    std::cout << "After inserts: "; print_set(numbers);

    auto [lo, hi] = numbers.equal_range(10);
    std::cout << "equal_range(10): ";
    for (auto it = lo; it != hi; ++it) std::cout << *it << " ";
    std::cout << "\n";

    std::cout << "\n=== vector → set conversion ===\n";
    const std::vector<int> raw{9, 8, 7, 7, 6, 5};
    const std::set<int> from_vec{raw.begin(), raw.end()};
    std::cout << "Converted: "; print_set(from_vec);

    std::cout << "\n=== copy map ===\n";
    const auto copied = ages;
    std::cout << "Copied:\n";
    print_map(copied);

    return 0;
}
