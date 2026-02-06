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
// SMALL ADDED CODE
// ----------------------------------------------------

// Helper to print user safely
void print_user(const std::unique_ptr<User>& user) {
    if (user) {
        std::cout << user->name << " " << user->age << '\n';
    } else {
        std::cout << "User is nullptr\n";
    }
}

// Small helper to update age
void birthday(User& user) {
    ++user.age;
}

// Stream output helper
std::ostream& operator<<(std::ostream& os, const User& u) {
    return os << u.name << " (" << u.age << ")";
}

// Small helper check
bool is_adult(const User& user) {
    return user.age >= 18;
}

// ---- very small extra helpers ----

// Factory helper
std::unique_ptr<User> make_user(std::string name, int age) {
    return std::make_unique<User>(User{std::move(name), age});
}

// Print adult status
void print_status(const User& user) {
    std::cout << (is_adult(user) ? "Adult\n" : "Minor\n");
}
// ---------------------------------

// ----------------------------------------------------
// MAIN (ORIGINAL + FEW ADDITIONS)
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
    birthday(*v);                  // increment age
    print_user(v);                 // safe printing

    std::cout << "Printed via operator<< : "
              << *v << std::endl;

    print_status(*v);

    // ---- very small extra usage ----
    auto w = make_user("Nova", 16);
    print_user(w);
    print_status(*w);
    // -------------------------------

    v.reset();                     // release ownership
    print_user(v);                 // check after reset

    return 0;
}
 
