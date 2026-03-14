// Creational Patterns Exercise
// Singleton, Factory, Builder

#include <iostream>
#include <memory>
#include <string>

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

class AnimalFactory {
public:
    static std::unique_ptr<Animal> create(const std::string& type) {
        if (type == "dog") return std::make_unique<Dog>();
        if (type == "cat") return std::make_unique<Cat>();
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

    Computer build() {
        return computer;
    }
};

// ---------------- Main ----------------

int main() {

    // Singleton usage
    Database::get_instance()->query("SELECT * FROM users");

    std::cout << "\n";

    // Factory usage
    auto dog = AnimalFactory::create("dog");
    auto cat = AnimalFactory::create("cat");

    if (dog) dog->speak();
    if (cat) cat->speak();

    std::cout << "\n";

    // Builder usage
    Computer pc = ComputerBuilder()
                    .set_cpu("Intel i7")
                    .set_ram("16GB")
                    .set_storage("1TB SSD")
                    .build();

    pc.show();

    return 0;
}
