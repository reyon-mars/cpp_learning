// Architectural Patterns Exercise
// MVC, Layers, Repository Pattern

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm> // tiny addition
#include <numeric>   // tiny addition

// Model
class User {
public:
    int id;
    std::string name;
    
    User(int id, const std::string& name) : id(id), name(name) {}
};

// Repository
class UserRepository {
private:
    std::vector<User> users;
    
public:
    void add(const User& user) {
        users.push_back(user);
    }
    
    User* find_by_id(int id) {
        for (auto& user : users) {
            if (user.id == id) return &user;
        }
        return nullptr;
    }
    
    const std::vector<User>& get_all() const {
        return users;
    }

    // ---- small addition ----
    bool remove_by_id(int id) {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->id == id) {
                users.erase(it);
                return true;
            }
        }
        return false;
    }

    // ---- VERY SMALL EXTRA ADDITIONS ----

    std::size_t count() const {
        return users.size();
    }

    bool exists(int id) const {
        for (const auto& user : users)
            if (user.id == id)
                return true;
        return false;
    }

    void clear() {
        users.clear();
    }

    // --- tiny new helper ---
    User* find_by_name(const std::string& name) {
        for (auto& user : users) {
            if (user.name == name) return &user;
        }
        return nullptr;
    }

    // ===== FINAL TINY ADDITIONS =====

    int total_ids() const {
        int sum = 0;
        for (const auto& user : users)
            sum += user.id;
        return sum;
    }

    bool empty() const {
        return users.empty();
    }

    // =================================
};

// View
class UserView {
public:
    void display_user(const User& user) {
        std::cout << "User: " << user.name << " (ID: " << user.id << ")\n";
    }

    // ---- small addition ----
    void display_all(const std::vector<User>& users) {
        std::cout << "--- User List ---\n";
        for (const auto& user : users) {
            display_user(user);
        }
    }

    void show_message(const std::string& msg) {
        std::cout << msg << "\n";
    }

    // ---- VERY SMALL EXTRA ADDITIONS ----

    void show_count(std::size_t count) {
        std::cout << "Total users: " << count << "\n";
    }

    void print_divider() {
        std::cout << "----------------------\n";
    }

    // --- tiny new helper ---
    void show_not_found(const std::string& type) {
        std::cout << type << " not found\n";
    }

    // ===== FINAL TINY ADDITIONS =====

    void show_total_ids(int sum) {
        std::cout << "Sum of user IDs: " << sum << "\n";
    }

    void show_empty(bool value) {
        std::cout << "Repository empty? "
                  << (value ? "Yes" : "No") << "\n";
    }

    // =================================
};

// Controller
class UserController {
private:
    UserRepository repository;
    UserView view;
    
public:
    void add_user(int id, const std::string& name) {
        repository.add(User(id, name));
    }
    
    void display_user(int id) {
        if (auto user = repository.find_by_id(id)) {
            view.display_user(*user);
        } else {
            view.show_not_found("User");
        }
    }

    // ---- small additions ----
    void list_users() {
        view.display_all(repository.get_all());
    }

    void remove_user(int id) {
        if (repository.remove_by_id(id))
            view.show_message("User removed");
        else
            view.show_message("User not found");
    }

    // ---- VERY SMALL EXTRA ADDITIONS ----

    void show_user_count() {
        view.show_count(repository.count());
    }

    void check_user(int id) {
        view.show_message(
            repository.exists(id) ? "User exists" : "User does not exist"
        );
    }

    void clear_users() {
        repository.clear();
        view.show_message("All users cleared");
    }

    void divider() {
        view.print_divider();
    }

    // --- tiny new feature ---
    void find_user_by_name(const std::string& name) {
        if (auto user = repository.find_by_name(name)) {
            view.display_user(*user);
        } else {
            view.show_not_found("User");
        }
    }

    // ===== FINAL TINY ADDITIONS =====

    void show_total_ids() {
        view.show_total_ids(repository.total_ids());
    }

    void show_if_empty() {
        view.show_empty(repository.empty());
    }

    // =================================
};

int main() {
    UserController controller;

    controller.add_user(1, "Alice");
    controller.add_user(2, "Bob");

    controller.display_user(1);

    // small extra usage
    controller.list_users();
    controller.remove_user(2);
    controller.list_users();

    // ---- VERY SMALL EXTRA USAGE ----
    controller.divider();

    controller.show_user_count();
    controller.check_user(1);
    controller.check_user(2);

    // --- tiny new usage ---
    controller.find_user_by_name("Alice");
    controller.find_user_by_name("Charlie");

    // ===== FINAL TINY USAGE =====

    controller.show_total_ids();
    controller.show_if_empty();

    controller.divider();

    // =================================

    controller.clear_users();
    controller.show_user_count();

    // ---- extra final check ----
    controller.show_if_empty();
    // ---------------------------

    return 0;
}
