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
// EXTRA HELPER FUNCTIONS
// ----------------------------------------------------

// Safely print a User
void print_user(const std::unique_ptr<User>& user) {
    if (user) {
        std::cout << "User: " << user->name
                  << ", age " << user->age << '\n';
    } else {
        std::cout << "User pointer is null\n";
    }
}

// Demonstrates ownership transfer
void ownership_transfer_demo() {
    std::cout << "\n[Extra] Ownership transfer demo\n";

    auto user1 = std::make_unique<User>(User{"Jupiter", 30});
    print_user(user1);

    std::unique_ptr<User> user2 = std::move(user1);
    print_user(user1);  // nullptr
    print_user(user2);  // owns object
}

// Factory function returning unique_ptr
std::unique_ptr<User> create_user(std::string name, int age) {
    return std::make_unique<User>(User{std::move(name), age});
}

// Demonstrates returning unique_ptr
void factory_demo() {
    std::cout << "\n[Extra] Factory function demo\n";
    auto u = create_user("Saturn", 45);
    print_user(u);
}

// Demonstrates reset()
void reset_demo() {
    std::cout << "\n[Extra] reset() demo\n";
    auto u = std::make_unique<User>(User{"Neptune", 50});
    print_user(u);

    u.reset();
    print_user(u);
}

// ----------------------------------------------------
// MAIN
// ----------------------------------------------------

int main(void) {

    // ---------- ORIGINAL LOGIC ----------
    auto u = std::make_unique<User>(User{"Mars", 22});
    std::cout << u->name << " " << u->age << '\n';

    std::unique_ptr<User> v;
    v = std::move(u);

    if (u == nullptr) {
        std::cout << "U is nullptr\n";
    }

    // ---------- EXTRA DEMOS ----------
    ownership_transfer_demo();
    factory_demo();
    reset_demo();

    return 0;
}

