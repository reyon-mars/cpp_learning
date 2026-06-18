#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <optional>

class User {
    int id_;
    std::string name_;

public:
    User(int id, std::string name) noexcept(false) : id_(id), name_(std::move(name)) {}

    [[nodiscard]] int id()                const noexcept { return id_;   }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }
};

class UserRepository {
    std::vector<User> users_;

public:
    void add(User user) {
        users_.push_back(std::move(user));
    }

    [[nodiscard]] User* find_by_id(int id) noexcept {
        auto it = std::ranges::find_if(users_, [id](const User& u) { return u.id() == id; });
        return it != users_.end() ? &*it : nullptr;
    }

    [[nodiscard]] User* find_by_name(std::string_view name) noexcept {
        auto it = std::ranges::find_if(users_, [name](const User& u) { return u.name() == name; });
        return it != users_.end() ? &*it : nullptr;
    }

    bool remove_by_id(int id) {
        auto it = std::ranges::find_if(users_, [id](const User& u) { return u.id() == id; });
        if (it == users_.end()) return false;
        users_.erase(it);
        return true;
    }

    [[nodiscard]] bool exists(int id) const noexcept {
        return std::ranges::any_of(users_, [id](const User& u) { return u.id() == id; });
    }

    [[nodiscard]] int total_ids() const noexcept {
        return std::accumulate(users_.begin(), users_.end(), 0,
            [](int sum, const User& u) { return sum + u.id(); });
    }

    void clear() noexcept { users_.clear(); }

    [[nodiscard]] std::size_t count() const noexcept { return users_.size(); }
    [[nodiscard]] bool empty()        const noexcept { return users_.empty(); }
    [[nodiscard]] const std::vector<User>& all() const noexcept { return users_; }
};

class UserView {
public:
    void display_user(const User& user) const {
        std::cout << "User: " << user.name() << " (ID: " << user.id() << ")\n";
    }

    void display_all(std::span<const User> users) const {
        std::cout << "--- User List ---\n";
        for (const auto& user : users) display_user(user);
    }

    void show_message(std::string_view msg)        const { std::cout << msg << '\n'; }
    void show_count(std::size_t count)              const { std::cout << "Total users: " << count << '\n'; }
    void show_not_found(std::string_view what)      const { std::cout << what << " not found\n"; }
    void show_total_ids(int sum)                    const { std::cout << "Sum of user IDs: " << sum << '\n'; }
    void show_empty(bool value)                     const { std::cout << "Repository empty? " << (value ? "Yes" : "No") << '\n'; }
    void print_divider()                            const { std::cout << "----------------------\n"; }
};

class UserController {
    UserRepository repository_;
    UserView       view_;

public:
    void add_user(int id, std::string name) {
        repository_.add(User(id, std::move(name)));
    }

    void display_user(int id) {
        if (auto* user = repository_.find_by_id(id))
            view_.display_user(*user);
        else
            view_.show_not_found("User");
    }

    void find_user_by_name(std::string_view name) {
        if (auto* user = repository_.find_by_name(name))
            view_.display_user(*user);
        else
            view_.show_not_found("User");
    }

    void list_users()         const { view_.display_all(repository_.all()); }
    void show_user_count()    const { view_.show_count(repository_.count()); }
    void show_total_ids()     const { view_.show_total_ids(repository_.total_ids()); }
    void show_if_empty()      const { view_.show_empty(repository_.empty()); }
    void divider()            const { view_.print_divider(); }

    void remove_user(int id) {
        view_.show_message(repository_.remove_by_id(id) ? "User removed" : "User not found");
    }

    void check_user(int id) const {
        view_.show_message(repository_.exists(id) ? "User exists" : "User does not exist");
    }

    void clear_users() {
        repository_.clear();
        view_.show_message("All users cleared");
    }
};

int main() {
    UserController controller;

    controller.add_user(1, "Alice");
    controller.add_user(2, "Bob");

    controller.display_user(1);

    controller.list_users();
    controller.remove_user(2);
    controller.list_users();

    controller.divider();

    controller.show_user_count();
    controller.check_user(1);
    controller.check_user(2);

    controller.find_user_by_name("Alice");
    controller.find_user_by_name("Charlie");

    controller.show_total_ids();
    controller.show_if_empty();

    controller.divider();

    controller.clear_users();
    controller.show_user_count();
    controller.show_if_empty();

    return 0;
}
