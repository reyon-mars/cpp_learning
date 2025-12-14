#include <iostream>
#include <memory>

struct User{
    std::string name;
    int age;
};

int main ( void ){
    auto u = std::make_unique<User>( "Mars", 22 );

    std::cout << u->name << " " << u->age << '\n';

    std::unique_ptr<User> v;
    v = std::move( u );

    if( u == nullptr ){
        std::cout << "U is nullptr ";
    }
    return 0;
}

// ----------------------------------------------------
// EXTRA CODE ADDED BELOW (original code untouched)
// ----------------------------------------------------

// Helper function to print a User safely
void print_user(const std::unique_ptr<User>& user) {
    if (user) {
        std::cout << "\n[Extra] User: "
                  << user->name << ", age " << user->age << '\n';
    } else {
        std::cout << "\n[Extra] User pointer is null\n";
    }
}

// Demonstrate ownership transfer again
int runExtraUniquePtrDemo() {
    auto user1 = std::make_unique<User>(User{"Jupiter", 30});
    print_user(user1);

    std::unique_ptr<User> user2 = std::move(user1);
    print_user(user1);   // now null
    print_user(user2);   // owns the object

    return 0;
}

// Automatically executed before main()
int __ = runExtraUniquePtrDemo();
