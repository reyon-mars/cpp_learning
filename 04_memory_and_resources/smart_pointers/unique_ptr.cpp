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

// Take ownership example
void take_ownership(std::unique_ptr<User> user) {
    std::cout << "Ownership taken: " << *user << '\n';
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

    // Raw pointer access (non-owning)
    User* raw = v.get();
    if (raw)
        std::cout << "Access via raw pointer: "
                  << raw->name << '\n';

    // Transfer ownership to function
    take_ownership(std::move(v));

    if (!v)
        std::cout << "v is now nullptr after move\n";

    // Reassign after move
    v = std::make_unique<User>(User{"Nova", 17});
    print_user(v);

    // Release demo
    User* released = v.release();
    if (!v)
        std::cout << "v released ownership\n";

    delete released;  // manual cleanup

    return 0;
}
