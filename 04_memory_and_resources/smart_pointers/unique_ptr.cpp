#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>
#include <numeric>

struct User {
    std::string name;
    int         age;
};

std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << u.name << " (" << u.age << ")";
}

void print_user(const std::unique_ptr<User>& user) {
    if (user) std::cout << *user << "\n";
    else      std::cout << "nullptr\n";
}

void birthday(User& user) noexcept { ++user.age; }

[[nodiscard]] bool is_adult(const User& user) noexcept { return user.age >= 18; }

void take_ownership(std::unique_ptr<User> user) {
    std::cout << "Ownership taken: " << *user << "\n";
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
    auto it = std::ranges::find_if(users,
        [target](const auto& u) { return u && u->name == target; });
    return it != users.end() ? it->get() : nullptr;
}

void sort_by_age(UserVec& users) {
    std::ranges::sort(users, {},
        [](const auto& u) { return u->age; });
}

void print_all(const UserVec& users) {
    for (const auto& u : users) { print_user(u); }
}

int main() {
    std::cout << "=== Basic unique_ptr ===\n";
    auto u = std::make_unique<User>(User{"Mars", 22});
    std::cout << *u << "\n";

    auto v = std::move(u);
    std::cout << "u is null: " << std::boolalpha << (u == nullptr) << "\n";
    assert(v != nullptr);

    birthday(*v);
    print_user(v);
    std::cout << "operator<<: " << *v << "\n";
    std::cout << "Adult: " << is_adult(*v) << "\n";

    if (User* raw = v.get()) {
        std::cout << "Raw pointer: " << raw->name << "\n";
    }

    take_ownership(std::move(v));
    std::cout << "v after move: " << (v == nullptr) << "\n";

    v = std::make_unique<User>(User{"Nova", 17});
    print_user(v);

    User* released = v.release();
    std::cout << "v after release: " << (v == nullptr) << "\n";
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
    users.reserve(4);
    users.push_back(create_user("A", 10));
    users.push_back(create_user("B", 20));
    users.push_back(create_user("C", 30));
    print_all(users);

    std::cout << "\n=== Algorithms ===\n";
    std::cout << "Adults: " << count_adults(users) << "\n";

    if (auto* oldest = find_oldest(users)) {
        std::cout << "Oldest: " << *oldest << "\n";
    }

    if (auto* youngest = find_youngest(users)) {
        std::cout << "Youngest: " << *youngest << "\n";
    }

    std::cout << "Average age: " << average_age(users) << "\n";

    if (auto* found = find_by_name(users, "B")) {
        std::cout << "Found: " << *found << "\n";
    }

    sort_by_age(users);
    std::cout << "Sorted:\n";
    print_all(users);

    users.push_back(create_user("D", 50));
    std::cout << "After push_back D:\n";
    print_all(users);

    std::cout << "\n=== Lambda + const ref ===\n";
    const auto& cref = users[0];
    std::cout << "Const access: " << *cref << "\n";

    std::ranges::for_each(users, [](const auto& u) {
        if (u) std::cout << "Lambda: " << *u << "\n";
    });

    return 0;
}
