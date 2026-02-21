#include <iostream>
#include <memory>
#include <string>

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
    birthday(*v);
    print_user(v);

    std::cout << "Printed via operator<<: "
              << *v << std::endl;

    std::cout << "Adult? "
              << (is_adult(*v) ? "Yes\n" : "No\n");

    v.reset();
    print_user(v);

    return 0;
}
