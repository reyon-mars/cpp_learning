#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>   // ✅ ADDED
#include <cassert>   // ✅ ADDED
#include <numeric>   // ✅ ADDED

// ----------------------------------------------------
// ORIGINAL STRUCT (LOGIC UNCHANGED)
// ----------------------------------------------------

struct User {
    std::string name;
    int age;
};

// ----------------------------------------------------
// SMALL ADDED CODE (HELPERS ONLY)
// ----------------------------------------------------

// Print user safely
void print_user(const std::unique_ptr<User>& user) {
    if (user)
        std::cout << user->name << " " << user->age << '\n';
    else
        std::cout << "User is nullptr\n";
}

// Increment age
void birthday(User& user) {
    ++user.age;
}

// Check adult status
bool is_adult(const User& user) {
    return user.age >= 18;
}

// Stream output helper
std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << u.name << " (" << u.age << ")";
}

// Take ownership example
void take_ownership(std::unique_ptr<User> user) {
    std::cout << "Ownership taken: " << *user << '\n';
}

// -------- NEW ADDITIONS --------

// Factory function
std::unique_ptr<User> create_user(std::string name, int age) {
    return std::make_unique<User>(User{name, age});
}

// Safe check helper
bool is_null(const std::unique_ptr<User>& u) {
    return !u;
}

// 🔹 Find oldest user
User* find_oldest(const std::vector<std::unique_ptr<User>>& users) {
    if (users.empty()) return nullptr;

    return std::max_element(users.begin(), users.end(),
        [](const auto& a, const auto& b) {
            return a->age < b->age;
        })->get();
}

// 🔹 Count adults
int count_adults(const std::vector<std::unique_ptr<User>>& users) {
    return std::count_if(users.begin(), users.end(),
        [](const auto& u) {
            return u && u->age >= 18;
        });
}

// 🔹 Print all users
void print_all(const std::vector<std::unique_ptr<User>>& users) {
    for (const auto& u : users) {
        print_user(u);
    }
}

// ✅ ADDED: find youngest user
User* find_youngest(const std::vector<std::unique_ptr<User>>& users) {
    if (users.empty()) return nullptr;

    return std::min_element(users.begin(), users.end(),
        [](const auto& a, const auto& b) {
            return a->age < b->age;
        })->get();
}

// ✅ ADDED: average age
double average_age(const std::vector<std::unique_ptr<User>>& users) {
    if (users.empty()) return 0.0;

    int total = std::accumulate(users.begin(), users.end(), 0,
        [](int sum, const auto& u) {
            return sum + (u ? u->age : 0);
        });

    return static_cast<double>(total) / users.size();
}

// ✅ ADDED: search user by name
User* find_user_by_name(
    const std::vector<std::unique_ptr<User>>& users,
    const std::string& target) {

    auto it = std::find_if(users.begin(), users.end(),
        [&](const auto& u) {
            return u && u->name == target;
        });

    return (it != users.end()) ? it->get() : nullptr;
}

// ✅ ADDED: sort users by age
void sort_users_by_age(
    std::vector<std::unique_ptr<User>>& users) {

    std::sort(users.begin(), users.end(),
        [](const auto& a, const auto& b) {
            return a->age < b->age;
        });
}

// --------------------------------

// ----------------------------------------------------
// MAIN
// ----------------------------------------------------

int main(void) {

    // ---------- ORIGINAL CODE ----------
    auto u = std::make_unique<User>(User{"Mars", 22});
    std::cout << u->name << " " << u->age << '\n';

    std::unique_ptr<User> v;
    v = std::move(u);

    if (u == nullptr) {
        std::cout << "U is nullptr\n";
    }

    // ---------- SMALL ADDITIONS ----------
    assert(v);  // ✅ ADDED safety check

    birthday(*v);
    print_user(v);

    std::cout << "Printed via operator<<: "
              << *v << std::endl;

    std::cout << "Adult? "
              << (is_adult(*v) ? "Yes\n" : "No\n");

    User* raw = v.get();
    if (raw)
        std::cout << "Access via raw pointer: "
                  << raw->name << '\n';

    take_ownership(std::move(v));

    if (!v)
        std::cout << "v is now nullptr after move\n";

    v = std::make_unique<User>(User{"Nova", 17});
    print_user(v);

    User* released = v.release();
    if (!v)
        std::cout << "v released ownership\n";

    delete released;

    // -------- NEW FEATURE USAGE --------

    // reset (safe replacement)
    v = create_user("Alex", 30);
    print_user(v);

    v.reset(new User{"ResetUser", 40});
    print_user(v);

    // swap ownership
    auto v2 = create_user("SwapUser", 25);
    std::swap(v, v2);

    std::cout << "After swap:\n";
    print_user(v);
    print_user(v2);

    // vector of unique_ptr
    std::vector<std::unique_ptr<User>> users;
    users.push_back(create_user("A", 10));
    users.push_back(create_user("B", 20));
    users.push_back(create_user("C", 30));

    std::cout << "Users in vector:\n";
    for (const auto& user : users) {
        print_user(user);
    }

    // null check helper
    if (is_null(v2))
        std::cout << "v2 is null\n";

    // -------- EXTRA NEW USAGE --------

    std::cout << "\n--- Advanced Utilities ---\n";

    // Print all users
    print_all(users);

    // Count adults
    std::cout << "Adult count: "
              << count_adults(users) << "\n";

    // Find oldest user
    if (auto oldest = find_oldest(users)) {
        std::cout << "Oldest user: "
                  << oldest->name << " ("
                  << oldest->age << ")\n";
    }

    // const unique_ptr usage
    const std::unique_ptr<User>& cref = users[0];
    std::cout << "Const access: " << *cref << "\n";

    // Lambda with unique_ptr
    std::for_each(users.begin(), users.end(),
        [](const auto& u) {
            if (u)
                std::cout << "Lambda user: "
                          << *u << "\n";
        });

    // -------- EXTRA SMALL ADDITIONS --------

    std::cout << "\n--- Extra Features ---\n";

    // Find youngest user
    if (auto youngest = find_youngest(users)) {
        std::cout << "Youngest user: "
                  << youngest->name << " ("
                  << youngest->age << ")\n";
    }

    // Average age
    std::cout << "Average age: "
              << average_age(users) << "\n";

    // Search by name
    if (auto found = find_user_by_name(users, "B")) {
        std::cout << "Found user: "
                  << found->name << "\n";
    }

    // Sort users
    sort_users_by_age(users);

    std::cout << "Sorted users by age:\n";
    print_all(users);

    // emplace_back usage
    users.emplace_back(create_user("D", 50));

    std::cout << "After emplace_back:\n";
    print_all(users);

    // ----------------------------------

    return 0;
}
