// Creational Patterns Exercise
// Singleton, Factory, Builder

#include <iostream>
#include <memory>

// Singleton
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

// Factory
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

int main() {
    Database::get_instance()->query("SELECT * FROM users");
    
    auto dog = AnimalFactory::create("dog");
    dog->speak();
    
    return 0;
}
