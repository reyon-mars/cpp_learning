#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <vector>

// ---------------- Singleton ----------------

class Database {
private:
    static Database* instance;
    Database() { std::cout << "DB initialized\n"; }
    
public:
    static Database* get_instance() {
        if (!instance) {
            instance = new Database();
        }
        return instance;
    }

    // Thread-safe access
    static Database* get_thread_safe_instance() {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance) {
            instance = new Database();
        }
        return instance;
    }

    // ---- VERY SMALL ADDITION ----
    static bool is_initialized() {
        return instance != nullptr;
    }
    // --------------------------------
    
    void query(const std::string& q) {
        std::cout << "Executing: " << q << "\n";
    }
};

Database* Database::instance = nullptr;

// ---------------- Factory ----------------

class Animal {
public:
    virtual ~Animal() = default;
    virtual void speak() = 0;
};

class Dog : public Animal {
public:
    void speak() override { std::cout << "Woof!\n"; }
};

class Cat : public Animal {
public:
    void speak() override { std::cout << "Meow!\n"; }
};

class Cow : public Animal {
public:
    void speak() override { std::cout << "Moo!\n"; }
};

class AnimalFactory {
public:
    static std::unique_ptr<Animal> create(const std::string& type) {
        if (type == "dog") return std::make_unique<Dog>();
        if (type == "cat") return std::make_unique<Cat>();
        if (type == "cow") return std::make_unique<Cow>();
        return nullptr;
    }
};

// ---------------- Builder ----------------

class Computer {
public:
    std::string cpu;
    std::string ram;
    std::string storage;

    void show() const {
        std::cout << "Computer specs:\n";
        std::cout << "CPU: " << cpu << "\n";
        std::cout << "RAM: " << ram << "\n";
        std::cout << "Storage: " << storage << "\n";
    }

    // ---- VERY SMALL ADDITION ----
    bool is_complete() const {
        return !cpu.empty() && !ram.empty() && !storage.empty();
    }
    // --------------------------------
};

class ComputerBuilder {
private:
    Computer computer;

public:
    ComputerBuilder& set_cpu(const std::string& c) {
        computer.cpu = c;
        return *this;
    }

    ComputerBuilder& set_ram(const std::string& r) {
        computer.ram = r;
        return *this;
    }

    ComputerBuilder& set_storage(const std::string& s) {
        computer.storage = s;
        return *this;
    }

    ComputerBuilder& gaming_pc() {
        computer.cpu = "Ryzen 9";
        computer.ram = "32GB";
        computer.storage = "2TB NVMe";
        return *this;
    }

    bool is_valid() const {
        return !computer.cpu.empty() &&
               !computer.ram.empty() &&
               !computer.storage.empty();
    }

    Computer build() {
        if (!is_valid()) {
            std::cout << "Warning: Incomplete build\n";
        }
        return computer;
    }
};

// ---------------- Helper Functions ----------------

// Thread function for singleton demo
void thread_task(int id) {
    Database* db = Database::get_thread_safe_instance();
    db->query("Thread " + std::to_string(id) + " query");
}

// Factory bulk test
void test_factory() {
    std::vector<std::string> types = {"dog", "cat", "cow", "unknown"};

    for (const auto& t : types) {
        auto animal = AnimalFactory::create(t);
        if (animal) {
            animal->speak();
        } else {
            std::cout << "Unknown animal type: " << t << "\n";
        }
    }
}

// ---- VERY SMALL ADDITION ----
void print_divider() {
    std::cout << "-----------------------------\n";
}
// --------------------------------

// ---------------- Main ----------------

int main() {

    // Singleton usage
    Database::get_instance()->query("SELECT * FROM users");

    // Check initialization
    std::cout << "DB initialized? "
              << (Database::is_initialized() ? "Yes" : "No") << "\n";

    // Thread-safe singleton usage
    Database::get_thread_safe_instance()->query("SELECT * FROM products");

    print_divider();

    std::cout << "\n--- Singleton Threads ---\n";

    std::thread t1(thread_task, 1);
    std::thread t2(thread_task, 2);
    std::thread t3(thread_task, 3);

    t1.join();
    t2.join();
    t3.join();

    print_divider();

    // Factory usage
    auto dog = AnimalFactory::create("dog");
    auto cat = AnimalFactory::create("cat");
    auto cow = AnimalFactory::create("cow");

    if (dog) dog->speak();
    if (cat) cat->speak();
    if (cow) cow->speak();

    std::cout << "\n--- Factory Bulk Test ---\n";
    test_factory();

    print_divider();

    // Builder usage
    Computer pc = ComputerBuilder()
                    .set_cpu("Intel i7")
                    .set_ram("16GB")
                    .set_storage("1TB SSD")
                    .build();

    pc.show();
    std::cout << "Complete? " << (pc.is_complete() ? "Yes" : "No") << "\n";

    print_divider();

    // Using preset builder
    Computer gaming = ComputerBuilder()
                        .gaming_pc()
                        .build();

    gaming.show();

    print_divider();

    std::cout << "\n--- Invalid Build Demo ---\n";

    Computer incomplete = ComputerBuilder()
                            .set_cpu("Only CPU")
                            .build();

    incomplete.show();
    std::cout << "Complete? "
              << (incomplete.is_complete() ? "Yes" : "No") << "\n";

    return 0;
}
