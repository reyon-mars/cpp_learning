// Creational Patterns Exercise
// Singleton, Factory, Builder

#include <iostream>
#include <memory>
#include <string>
#include <mutex>   // ✅ ADDED for thread safety

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

    // ✅ ADDED: Thread-safe access (extra, original unchanged)
    static Database* get_thread_safe_instance() {
        static std::mutex mtx;
        std::lock_guard<std::mutex> lock(mtx);
        if (!instance) {
            instance = new Database();
        }
        return instance;
    }
    
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

// ✅ ADDED: New Animal
class Cow : public Animal {
public:
    void speak() override { std::cout << "Moo!\n"; }
};

class AnimalFactory {
public:
    static std::unique_ptr<Animal> create(const std::string& type) {
        if (type == "dog") return std::make_unique<Dog>();
        if (type == "cat") return std::make_unique<Cat>();
        if (type == "cow") return std::make_unique<Cow>(); // ✅ ADDED
        return nullptr;
    }
};

// ---------------- Builder (Small Addition) ----------------

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

    // ✅ ADDED: Preset configuration
    ComputerBuilder& gaming_pc() {
        computer.cpu = "Ryzen 9";
        computer.ram = "32GB";
        computer.storage = "2TB NVMe";
        return *this;
    }

    Computer build() {
        return computer;
    }
};

// ---------------- Main ----------------

int main() {

    // Singleton usage
    Database::get_instance()->query("SELECT * FROM users");

    // ✅ ADDED: Thread-safe singleton usage
    Database::get_thread_safe_instance()->query("SELECT * FROM products");

    std::cout << "\n";

    // Factory usage
    auto dog = AnimalFactory::create("dog");
    auto cat = AnimalFactory::create("cat");
    auto cow = AnimalFactory::create("cow"); // ✅ ADDED

    if (dog) dog->speak();
    if (cat) cat->speak();
    if (cow) cow->speak(); // ✅ ADDED

    std::cout << "\n";

    // Builder usage
    Computer pc = ComputerBuilder()
                    .set_cpu("Intel i7")
                    .set_ram("16GB")
                    .set_storage("1TB SSD")
                    .build();

    pc.show();

    std::cout << "\n";

    // ✅ ADDED: Using preset builder
    Computer gaming = ComputerBuilder()
                        .gaming_pc()
                        .build();

    gaming.show();

    return 0;
}
