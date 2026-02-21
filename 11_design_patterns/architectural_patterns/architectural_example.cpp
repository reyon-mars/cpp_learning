// Architectural Patterns Exercise
// MVC, Layers, Repository Pattern

#include <iostream>
#include <vector>
#include <memory>

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
};

// View
class UserView {
public:
    void display_user(const User& user) {
        std::cout << "User: " << user.name << " (ID: " << user.id << ")\n";
    }
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
};

int main() {
    UserController controller;
    controller.add_user(1, "Alice");
    controller.add_user(2, "Bob");
    controller.display_user(1);
    
    return 0;
}
