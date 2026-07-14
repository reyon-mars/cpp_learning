#include <forward_list>
#include <iostream>
#include <string>
#include <string_view>
#include <optional>
#include <algorithm>
#include <numeric>
#include <cstddef>
#include <vector>
#include <ranges>
#include <cassert>

void print_list(const std::forward_list<std::string>& lst, std::string_view label = "") {
    if (!label.empty()) std::cout << label << ": ";
    for (const auto& s : lst) std::cout << s << " ";
    std::cout << "\n";
}

[[nodiscard]] std::size_t list_size(const std::forward_list<std::string>& lst) noexcept {
    return static_cast<std::size_t>(std::ranges::distance(lst));
}

[[nodiscard]] bool contains(const std::forward_list<std::string>& lst, std::string_view value) {
    return std::ranges::find(lst, value) != lst.end();
}

[[nodiscard]] std::optional<std::string> last_element(const std::forward_list<std::string>& lst) {
    if (lst.empty()) return std::nullopt;
    auto it = lst.begin();
    while (std::next(it) != lst.end()) ++it;
    return *it;
}

[[nodiscard]] std::size_t count_value(const std::forward_list<std::string>& lst, std::string_view value) {
    return static_cast<std::size_t>(std::ranges::count(lst, value));
}

[[nodiscard]] bool all_non_empty(const std::forward_list<std::string>& lst) {
    return std::ranges::all_of(lst, [](const std::string& s) { return !s.empty(); });
}

[[nodiscard]] std::size_t total_characters(const std::forward_list<std::string>& lst) {
    return std::accumulate(lst.begin(), lst.end(), std::size_t{0},
        [](std::size_t acc, const std::string& s) { return acc + s.size(); });
}

[[nodiscard]] std::optional<std::string> find_longest(const std::forward_list<std::string>& lst) {
    if (lst.empty()) return std::nullopt;
    auto it = std::ranges::max_element(lst, {}, &std::string::size);
    return *it;
}

[[nodiscard]] std::optional<std::string> find_shortest(const std::forward_list<std::string>& lst) {
    if (lst.empty()) return std::nullopt;
    auto it = std::ranges::min_element(lst, {}, &std::string::size);
    return *it;
}

[[nodiscard]] std::vector<std::string> to_vector(const std::forward_list<std::string>& lst) {
    return std::vector<std::string>(lst.begin(), lst.end());
}

[[nodiscard]] std::forward_list<std::string> from_vector(const std::vector<std::string>& v) {
    std::forward_list<std::string> result;
    for (auto it = v.rbegin(); it != v.rend(); ++it)
        result.push_front(*it);
    return result;
}

[[nodiscard]] bool starts_with_any(const std::forward_list<std::string>& lst,
                                    std::string_view prefix) {
    return std::ranges::any_of(lst, [prefix](const std::string& s) {
        return s.starts_with(prefix);
    });
}

void print_first_n(const std::forward_list<std::string>& lst, int n) {
    int i = 0;
    for (const auto& s : lst) {
        if (i++ >= n) break;
        std::cout << s << " ";
    }
    std::cout << "\n";
}

void find_and_add(std::forward_list<std::string>& lst,
                  std::string_view target,
                  std::string replacement) {
    auto prev = lst.before_begin();
    for (auto curr = lst.begin(); curr != lst.end(); prev = curr, ++curr) {
        if (*curr == target) {
            lst.erase_after(prev);
            lst.insert_after(prev, std::move(replacement));
            return;
        }
    }
    auto tail = lst.before_begin();
    for (auto it = lst.begin(); it != lst.end(); ++it) tail = it;
    lst.insert_after(tail, std::move(replacement));
}

void transform_in_place(std::forward_list<std::string>& lst, auto fn) {
    for (auto& s : lst) s = fn(std::move(s));
}

[[nodiscard]] std::forward_list<std::string> merge_sorted(
    std::forward_list<std::string> a,
    std::forward_list<std::string> b) {
    a.sort();
    b.sort();
    a.merge(b);
    return a;
}

int main() {
    std::forward_list<std::string> lst{"apple", "banana", "cherry", "date"};

    std::cout << "=== Initial ===\n";
    print_list(lst);
    std::cout << "size=" << list_size(lst) << "\n";

    std::cout << "\n=== find_and_add ===\n";
    find_and_add(lst, "banana", "blueberry");
    print_list(lst, "Replace banana");

    find_and_add(lst, "apple", "apricot");
    print_list(lst, "Replace apple");

    find_and_add(lst, "date", "dragonfruit");
    print_list(lst, "Replace date");

    find_and_add(lst, "kiwi", "kumquat");
    print_list(lst, "Insert kumquat (not found)");
    std::cout << "size=" << list_size(lst) << "\n";

    std::cout << "\n=== Query helpers ===\n";
    std::cout << "empty="           << std::boolalpha << lst.empty()            << "\n"
              << "front="           << lst.front()                               << "\n"
              << "contains(cherry)=" << contains(lst, "cherry")                  << "\n"
              << "count(cherry)="   << count_value(lst, "cherry")               << "\n"
              << "all_non_empty="   << all_non_empty(lst)                       << "\n"
              << "total_chars="     << total_characters(lst)                    << "\n";
    if (auto last = last_element(lst)) std::cout << "last=" << *last << "\n";

    std::cout << "\n=== find_longest / find_shortest ===\n";
    if (auto l = find_longest(lst))  std::cout << "longest="  << *l << "\n";
    if (auto s = find_shortest(lst)) std::cout << "shortest=" << *s << "\n";

    std::cout << "\n=== starts_with_any(a) ===\n";
    std::cout << "starts_with 'a'=" << starts_with_any(lst, "a") << "\n"
              << "starts_with 'z'=" << starts_with_any(lst, "z") << "\n";

    std::cout << "\n=== to_vector / from_vector round-trip ===\n";
    auto vec = to_vector(lst);
    std::cout << "vector size=" << vec.size() << "\n";
    auto lst2 = from_vector(vec);
    print_list(lst2, "from_vector");

    std::cout << "\n=== push_front / pop_front ===\n";
    lst.push_front("first");
    print_list(lst, "After push_front");
    lst.pop_front();
    print_list(lst, "After pop_front");

    std::cout << "\n=== remove / reverse / sort ===\n";
    lst.remove("cherry");
    print_list(lst, "After remove(cherry)");
    lst.reverse();
    print_list(lst, "Reversed");
    lst.sort();
    print_list(lst, "Sorted");

    std::cout << "\n=== remove_if / unique ===\n";
    lst.remove_if([](const std::string& s) { return s.size() < 6; });
    print_list(lst, "After remove_if(len<6)");

    lst.push_front("apple");
    lst.push_front("apple");
    lst.unique();
    print_list(lst, "After unique");

    std::cout << "\n=== splice_after ===\n";
    std::forward_list<std::string> extra{"x", "y"};
    lst.splice_after(lst.before_begin(), extra);
    print_list(lst, "After splice_after");
    std::cout << "extra empty=" << extra.empty() << "\n";

    std::cout << "\n=== print_first_n(3) ===\n";
    print_first_n(lst, 3);

    std::cout << "\n=== emplace_front / resize ===\n";
    lst.emplace_front("starter");
    print_list(lst, "After emplace_front");
    lst.resize(5);
    print_list(lst, "After resize(5)");

    std::cout << "\n=== transform_in_place (uppercase first char) ===\n";
    std::forward_list<std::string> words{"cat", "dog", "eel"};
    transform_in_place(words, [](std::string s) {
        if (!s.empty()) s[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[0])));
        return s;
    });
    print_list(words, "Capitalised");

    std::cout << "\n=== merge_sorted ===\n";
    std::forward_list<std::string> ma{"banana", "date"};
    std::forward_list<std::string> mb{"apricot", "cherry", "elderberry"};
    auto merged = merge_sorted(std::move(ma), std::move(mb));
    print_list(merged, "Merged sorted");

    assert(!contains(merged, "kiwi"));
    assert(contains(merged, "cherry"));
    assert(find_longest(merged).has_value());

    std::cout << "\n=== clear ===\n";
    lst.clear();
    std::cout << "size=" << list_size(lst) << " empty=" << lst.empty() << "\n";

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
