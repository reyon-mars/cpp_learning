#include <iostream>
#include <memory>
#include <string>

// ----------------------------------------------------
// ORIGINAL STRUCT (UNCHANGED)
// ----------------------------------------------------

struct User {
    std::string name;
    int age;
};

// ----------------------------------------------------
// SMALL EXTRA CODE (ADDED)
// ----------------------------------------------------

// Safe helper to print User details
void print_user(const std::unique_ptr<User>& user) {
    if (user) {
        std::cout << "User: " << user->name
                  << ", age " << user->age << '\n';
    } else {
        std::cout << "User pointer is null\n";
    }
}

// Small helper to increase age
void birthday(User& user) {
    user.age++;
}

// Simple demo function
void extra_demo() {
    std::cout << "\n[Extra Demo]\n";

    auto user = std::make_unique<User>(User{"Venus", 28});
    print_user(user);

    birthday(*user);   // small added behavior
    std::cout << "After birthday:\n";
    print_user(user);

    user.reset();      // release ownership
    print_user(user);
}

// ----------------------------------------------------
// MAIN (ORIGINAL LOGIC + SMALL ADDITION)
// ----------------------------------------------------

int main(void) {

    // ---------- ORIGINAL LOGIC ----------
    auto u = std::make_unique<User>(User{"Mars", 22});
    std::cout << u->name << " " << u->age << '\n';

    std::unique_ptr<User> v;
    v = std::move(u);   // transfer ownership

    if (u == nullptr) {
        std::cout << "U is nullptr\n";
    }

    // ---- small added usage ----
    birthday(*v);
    std::cout << "After birthday: "
              << v->name << " " << v->age << '\n';

    // ---------- SMALL EXTRA CALL ----------
    extra_demo();

    return 0;
}
