#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>
#include <numeric>
#include <ranges>
#include <format>
#include <span>

struct User {
    std::string name;
    int         age;
};

template <>
struct std::formatter<User> : std::formatter<std::string> {
    auto format(const User& u, std::format_context& ctx) const {
        return std::formatter<std::string>::format(
            std::format("{} ({})", u.name, u.age), ctx);
    }
};

std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << std::format("{}", u);
}

void print_user(const std::unique_ptr<User>& user) {
    std::cout << (user ? std::format("{}\n", *user) : "nullptr\n");
}

void birthday(User& user) noexcept { ++user.age; }

[[nodiscard]] bool is_adult(const User& user) noexcept { return user.age >= 18; }

void take_ownership(std::unique_ptr<User> user) {
    std::cout << std::format("Ownership taken: {}\n", *user);
}

[[nodiscard]] std::unique_ptr<User> create_user(std::string name, int age) {
    return std::make_unique<User>(User{std::move(name), age});
}

using UserVec = std::vector<std::unique_ptr<User>>;

[[nodiscard]] User* find_oldest(const UserVec& users) {
    if (users.empty()) return nullptr;
    return std::ranges::max_element(users, {},
        [](const auto& u) { return u->age; })->get();
}

[[nodiscard]] User* find_youngest(const UserVec& users) {
    if (users.empty()) return nullptr;
    return std::ranges::min_element(users, {},
        [](const auto& u) { return u->age; })->get();
}

[[nodiscard]] long count_adults(const UserVec& users) {
    return std::ranges::count_if(users,
        [](const auto& u) { return u && u->age >= 18; });
}

[[nodiscard]] double average_age(const UserVec& users) {
    if (users.empty()) return 0.0;
    const int total = std::accumulate(users.begin(), users.end(), 0,
        [](int sum, const auto& u) { return sum + (u ? u->age : 0); });
    return static_cast<double>(total) / static_cast<double>(users.size());
}

[[nodiscard]] User* find_by_name(const UserVec& users, std::string_view target) {
    const auto it = std::ranges::find_if(users,
        [target](const auto& u) { return u && u->name == target; });
    return it != users.end() ? it->get() : nullptr;
}

void sort_by_age(UserVec& users) {
    std::ranges::sort(users, {}, [](const auto& u) { return u->age; });
}

void sort_by_name(UserVec& users) {
    std::ranges::sort(users, {}, [](const auto& u) -> std::string_view { return u->name; });
}

void print_all(const UserVec& users) {
    for (const auto& u : users) print_user(u);
}

[[nodiscard]] std::vector<std::string> extract_names(const UserVec& users) {
    std::vector<std::string> names;
    names.reserve(users.size());
    std::ranges::transform(users, std::back_inserter(names),
        [](const auto& u) { return u ? u->name : "?"; });
    return names;
}

void remove_by_name(UserVec& users, std::string_view target) {
    const auto [first, last] = std::ranges::remove_if(users,
        [target](const auto& u) { return u && u->name == target; });
    users.erase(first, last);
}

[[nodiscard]] UserVec partition_adults(UserVec& users) {
    UserVec adults;
    for (auto& u : users)
        if (u && is_adult(*u))
            adults.push_back(std::move(u));
    const auto [first, last] = std::ranges::remove_if(users,
        [](const auto& u) { return !u; });
    users.erase(first, last);
    return adults;
}

[[nodiscard]] int age_of_oldest(const UserVec& users) {
    if (users.empty()) return 0;
    return std::ranges::max(users |
        std::views::transform([](const auto& u) { return u->age; }));
}

class UserRepo {
    UserVec users_;

public:
    void add(std::string name, int age) {
        users_.push_back(create_user(std::move(name), age));
    }

    [[nodiscard]] std::size_t size()        const noexcept { return users_.size(); }
    [[nodiscard]] bool        empty()       const noexcept { return users_.empty(); }
    [[nodiscard]] long        adult_count() const { return count_adults(users_); }
    [[nodiscard]] double      mean_age()    const { return average_age(users_); }
    [[nodiscard]] User*       by_name(std::string_view n) { return find_by_name(users_, n); }

    void sort_age()  { sort_by_age(users_);  }
    void sort_name() { sort_by_name(users_); }

    void birthday_all() noexcept {
        for (auto& u : users_) if (u) birthday(*u);
    }

    void print() const { print_all(users_); }

    void remove(std::string_view name) { remove_by_name(users_, name); }
};

int main() {
    std::cout << "=== Basic unique_ptr ===\n";
    auto u = std::make_unique<User>(User{"Mars", 22});
    std::cout << std::format("{}\n", *u);

    auto v = std::move(u);
    std::cout << std::format("u is null: {}\n", u == nullptr);
    assert(v != nullptr);

    birthday(*v);
    print_user(v);
    std::cout << std::format("operator<<: {}\n", *v);
    std::cout << std::format("Adult: {}\n", is_adult(*v));

    if (User* raw = v.get())
        std::cout << std::format("Raw pointer: {}\n", raw->name);

    take_ownership(std::move(v));
    std::cout << std::format("v after move: {}\n", v == nullptr);

    v = std::make_unique<User>(User{"Nova", 17});
    print_user(v);

    User* released = v.release();
    std::cout << std::format("v after release: {}\n", v == nullptr);
    delete released;

    std::cout << "\n=== reset / swap ===\n";
    v = create_user("Alex", 30);
    print_user(v);

    v.reset(new User{"ResetUser", 40});
    print_user(v);

    auto v2 = create_user("SwapUser", 25);
    std::swap(v, v2);
    std::cout << "After swap:\n";
    print_user(v);
    print_user(v2);

    std::cout << "\n=== UserVec ===\n";
    UserVec users;
    users.reserve(5);
    users.push_back(create_user("A", 10));
    users.push_back(create_user("B", 20));
    users.push_back(create_user("C", 30));
    print_all(users);

    std::cout << "\n=== Algorithms ===\n";
    std::cout << std::format("Adults: {}\n", count_adults(users));

    if (auto* oldest   = find_oldest(users))
        std::cout << std::format("Oldest: {}\n", *oldest);
    if (auto* youngest = find_youngest(users))
        std::cout << std::format("Youngest: {}\n", *youngest);

    std::cout << std::format("Average age: {}\n", average_age(users));

    if (auto* found = find_by_name(users, "B"))
        std::cout << std::format("Found: {}\n", *found);

    sort_by_age(users);
    std::cout << "Sorted by age:\n";
    print_all(users);

    users.push_back(create_user("D", 50));
    std::cout << "After push_back D:\n";
    print_all(users);

    std::cout << "\n=== Lambda + const ref ===\n";
    const auto& cref = users[0];
    std::cout << std::format("Const access: {}\n", *cref);

    std::ranges::for_each(users, [](const auto& u) {
        if (u) std::cout << std::format("Lambda: {}\n", *u);
    });

    std::cout << "\n=== extract_names ===\n";
    for (const auto& n : extract_names(users))
        std::cout << std::format("  {}\n", n);

    std::cout << "\n=== remove_by_name ===\n";
    remove_by_name(users, "B");
    std::cout << std::format("After removing B, size={}\n", users.size());
    print_all(users);

    std::cout << "\n=== sort_by_name ===\n";
    sort_by_name(users);
    print_all(users);

    std::cout << "\n=== age_of_oldest ===\n";
    std::cout << std::format("Oldest age: {}\n", age_of_oldest(users));

    std::cout << "\n=== partition adults/minors ===\n";
    UserVec adults = partition_adults(users);
    std::cout << std::format("Adults ({}):\n", adults.size());
    print_all(adults);
    std::cout << std::format("Minors ({}):\n", users.size());
    print_all(users);

    std::cout << "\n=== UserRepo ===\n";
    UserRepo repo;
    repo.add("Eve",   25);
    repo.add("Frank", 15);
    repo.add("Grace", 30);
    repo.add("Hank",  12);
    repo.print();
    std::cout << std::format("size={} adults={} mean_age={:.1f}\n",
                             repo.size(), repo.adult_count(), repo.mean_age());

    repo.sort_age();
    std::cout << "Sorted by age:\n";
    repo.print();

    repo.birthday_all();
    std::cout << "After birthday_all:\n";
    repo.print();

    repo.remove("Frank");
    std::cout << std::format("After removing Frank, size={}\n", repo.size());
    repo.sort_name();
    repo.print();

    if (auto* found = repo.by_name("Grace"))
        std::cout << std::format("Repo found: {}\n", *found);

    assert(repo.adult_count() >= 0);
    std::cout << "\nAll assertions passed.\n";
    return 0;
}
