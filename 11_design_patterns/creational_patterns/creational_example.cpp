#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm> // tiny addition
#include <numeric>   // tiny addition

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

    // ---- EXTRA SMALL ADDITIONS ----
    static void print_status() {
        std::cout << "Database status: "
                  << (instance ? "Active" : "Not initialized") << "\n";
    }

    void ping() {
        std::cout << "Database ping successful\n";
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

    // ---- EXTRA SMALL ADDITION ----
    static bool is_supported(const std::string& type) {
        return type == "dog" ||
               type == "cat" ||
               type == "cow";
    }
    // --------------------------------
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

    // ---- EXTRA SMALL ADDITION ----
    void summary() const {
        std::cout << cpu << " | "
                  << ram << " | "
                  << storage << "\n";
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

// ---- EXTRA SMALL HELPERS ----

// count supported animals
int count_supported(const std::vector<std::string>& types) {
    return std::count_if(types.begin(), types.end(),
        [](const std::string& t) {
            return AnimalFactory::is_supported(t);
        });
}

// print simple title
void print_title(const std::string& title) {
    std::cout << "\n=== " << title << " ===\n";
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

    // ---- EXTRA SMALL USAGE ----
    Database::print_status();
    Database::get_instance()->ping();
    // --------------------------------

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

    // ---- EXTRA SMALL USAGE ----
    std::vector<std::string> animals =
        {"dog", "cat", "cow", "bird"};

    std::cout << "Supported animal types: "
              << count_supported(animals) << "\n";
    // --------------------------------

    print_divider();

    // Builder usage
    Computer pc = ComputerBuilder()
                    .set_cpu("Intel i7")
                    .set_ram("16GB")
                    .set_storage("1TB SSD")
                    .build();

    pc.show();
    std::cout << "Complete? " << (pc.is_complete() ? "Yes" : "No") << "\n";

    // ---- EXTRA SMALL USAGE ----
    pc.summary();
    // --------------------------------

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

    // ===== FINAL SMALL ADDITIONS =====

    print_title("Numeric Demo");

    std::vector<int> nums = {1, 2, 3, 4, 5};

    int total = std::accumulate(nums.begin(), nums.end(), 0);

    std::cout << "Accumulated sum: "
              << total << "\n";

    std::cout << "Max value: "
              << *std::max_element(nums.begin(), nums.end())
              << "\n";

    std::cout << "Min value: "
              << *std::min_element(nums.begin(), nums.end())
              << "\n";

    // =================================

    return 0;
}
