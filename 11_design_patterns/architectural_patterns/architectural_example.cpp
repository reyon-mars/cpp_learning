// Architectural Patterns Exercise
// MVC, Layers, Repository Pattern

#include <iostream>
#include <vector>
#include <memory>
#include <string>

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
    // ------------------------
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
    // ------------------------
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
    // -------------------------
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

    return 0;
}
