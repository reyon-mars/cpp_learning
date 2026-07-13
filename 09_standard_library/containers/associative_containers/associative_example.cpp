#include <iostream>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <string_view>
#include <vector>
#include <format>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <numeric>
#include <optional>

void print_map(const std::map<std::string, int>& m) {
    for (const auto& [k, v] : m)
        std::cout << std::format("  {}={}\n", k, v);
}

template <std::ranges::input_range R>
void print_range(std::string_view label, const R& r) {
    std::cout << std::format("{}: ", label);
    for (const auto& v : r) std::cout << std::format("{} ", v);
    std::cout << '\n';
}

template <typename Map>
[[nodiscard]] std::optional<typename Map::mapped_type>
try_get(const Map& m, const typename Map::key_type& key) {
    if (const auto it = m.find(key); it != m.end())
        return it->second;
    return std::nullopt;
}

template <typename Map>
[[nodiscard]] std::vector<typename Map::key_type> keys(const Map& m) {
    std::vector<typename Map::key_type> result;
    result.reserve(m.size());
    std::ranges::transform(m, std::back_inserter(result),
                           [](const auto& kv) { return kv.first; });
    return result;
}

template <typename Map>
[[nodiscard]] std::vector<typename Map::mapped_type> values(const Map& m) {
    std::vector<typename Map::mapped_type> result;
    result.reserve(m.size());
    std::ranges::transform(m, std::back_inserter(result),
                           [](const auto& kv) { return kv.second; });
    return result;
}

int main() {
    std::cout << "=== std::map ===\n";
    std::map<std::string, int> ages{
        {"Alice", 30},
        {"Bob",   25},
    };

    std::cout << std::format("Alice={}\n", ages.at("Alice"));
    ages.emplace("Charlie", 28);
    ages.emplace("David",   40);

    std::cout << "All entries:\n";
    print_map(ages);

    if (auto it = ages.find("Bob"); it != ages.end())
        std::cout << std::format("Found Bob={}\n", it->second);

    std::cout << std::format("contains(Alice)={}\n", ages.contains("Alice"));

    ages.at("Alice") += 1;
    std::cout << std::format("Updated Alice={}\n", ages.at("Alice"));

    ages.erase("David");
    std::cout << "After erasing David:\n";
    print_map(ages);
    std::cout << std::format("size={}\n", ages.size());

    if (const auto v = try_get(ages, std::string("Bob")))
        std::cout << std::format("try_get Bob={}\n", *v);
    assert(!try_get(ages, std::string("David")).has_value());

    print_range("keys",   keys(ages));
    print_range("values", values(ages));

    std::cout << std::format("Sum of ages: {}\n",
                             std::reduce(ages.begin(), ages.end(), 0,
                                         [](int acc, const auto& kv) {
                                             return acc + kv.second;
                                         }));

    std::cout << "\n=== std::set ===\n";
    std::set<int> unique_values{5, 2, 8, 2, 5, 1};
    print_range("Unique", unique_values);
    std::cout << std::format("contains(5)={}\n", unique_values.contains(5));

    unique_values.erase(2);
    print_range("After erase(2)", unique_values);

    auto lb = unique_values.lower_bound(5);
    auto ub = unique_values.upper_bound(5);
    std::cout << std::format("lower_bound(5)={}\n",
                             lb != unique_values.end() ? std::to_string(*lb) : "end");
    std::cout << std::format("upper_bound(5)={}\n",
                             ub != unique_values.end() ? std::to_string(*ub) : "end");

    std::set<int> other_set{100, 200, 300};
    unique_values.swap(other_set);
    print_range("After swap", unique_values);
    unique_values.swap(other_set);
    print_range("Restored",   unique_values);
    std::cout << std::format("size={}\n", unique_values.size());

    std::cout << "\n=== std::multimap ===\n";
    std::multimap<std::string, int> scores{
        {"Alice", 90},
        {"Alice", 85},
        {"Bob",   88},
    };
    for (const auto& [name, score] : scores)
        std::cout << std::format("  {}={}\n", name, score);

    auto [first, last] = scores.equal_range("Alice");
    std::cout << "Alice scores: ";
    for (auto it = first; it != last; ++it)
        std::cout << std::format("{} ", it->second);
    std::cout << '\n';
    std::cout << std::format("count(Alice)={}\n", scores.count("Alice"));

    const double alice_avg = [&] {
        int sum = 0, cnt = 0;
        for (auto it = first; it != last; ++it, ++cnt) sum += it->second;
        return cnt ? static_cast<double>(sum) / cnt : 0.0;
    }();
    std::cout << std::format("Alice average score: {:.1f}\n", alice_avg);

    std::cout << "\n=== std::multiset ===\n";
    std::multiset<int> numbers{5, 2, 8, 2, 5, 1};
    print_range("Values", numbers);
    std::cout << std::format("count(5)={}\n", numbers.count(5));

    numbers.clear();
    std::cout << std::format("After clear: empty={} size={}\n",
                             numbers.empty(), numbers.size());

    for (int v : {10, 10, 20}) numbers.insert(v);
    print_range("After inserts", numbers);

    auto [lo, hi] = numbers.equal_range(10);
    std::cout << "equal_range(10): ";
    for (auto it = lo; it != hi; ++it) std::cout << std::format("{} ", *it);
    std::cout << '\n';

    std::cout << "\n=== vector → set conversion ===\n";
    const std::vector<int> raw{9, 8, 7, 7, 6, 5};
    const std::set<int> from_vec{raw.begin(), raw.end()};
    print_range("Converted", from_vec);

    std::cout << "\n=== copy map ===\n";
    const auto copied = ages;
    print_map(copied);

    std::cout << "\n=== unordered_map ===\n";
    std::unordered_map<std::string, int> umap{
        {"x", 10}, {"y", 20}, {"z", 30}
    };
    umap.emplace("w", 40);
    std::cout << std::format("umap[x]={}\n", umap.at("x"));
    std::cout << std::format("contains(y)={}\n", umap.contains("y"));
    umap.erase("z");
    std::cout << std::format("size after erase={}\n", umap.size());

    const int umap_total = std::reduce(umap.begin(), umap.end(), 0,
                                       [](int acc, const auto& kv) {
                                           return acc + kv.second;
                                       });
    std::cout << std::format("umap value sum={}\n", umap_total);

    std::cout << "\n=== unordered_set ===\n";
    std::unordered_set<int> uset{1, 2, 3, 4, 5};
    uset.insert(3);
    std::cout << std::format("size (no dup)={}\n", uset.size());
    std::cout << std::format("contains(4)={}\n", uset.contains(4));
    uset.erase(1);
    std::cout << std::format("size after erase={}\n", uset.size());

    std::cout << "\n=== set operations via ranges ===\n";
    const std::set<int> sa{1, 2, 3, 4};
    const std::set<int> sb{3, 4, 5, 6};

    std::vector<int> s_intersection, s_difference, s_union;
    std::ranges::set_intersection(sa, sb,  std::back_inserter(s_intersection));
    std::ranges::set_difference(sa, sb,    std::back_inserter(s_difference));
    std::ranges::set_union(sa, sb,         std::back_inserter(s_union));

    print_range("intersection", s_intersection);
    print_range("difference",   s_difference);
    print_range("union",        s_union);

    std::cout << "\n=== views over map ===\n";
    auto sorted_by_value = ages
        | std::views::transform([](const auto& kv) { return kv; })
        | std::ranges::to<std::vector>();
    std::ranges::sort(sorted_by_value, {}, [](const auto& kv) { return kv.second; });
    for (const auto& [k, v] : sorted_by_value)
        std::cout << std::format("  {}={}\n", k, v);

    assert(ages.contains("Alice"));
    assert(!ages.contains("David"));
    assert(try_get(ages, std::string("Alice")).has_value());
    assert(s_intersection == std::vector<int>{3, 4});
    assert(s_difference   == std::vector<int>{1, 2});
    assert(s_union.size() == 6);
    assert(umap.contains("w"));
    assert(uset.contains(4));

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
