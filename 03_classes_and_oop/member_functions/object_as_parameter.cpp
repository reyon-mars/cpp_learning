#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <algorithm>
#include <ranges>
#include <span>
#include <cassert>

class Pet {
    std::string name_;

public:
    explicit Pet(std::string name) : name_(std::move(name)) {}

    virtual ~Pet() {
        std::cout << "~Pet: " << name_ << '\n';
    }

    Pet(const Pet&)            = default;
    Pet& operator=(const Pet&) = default;
    Pet(Pet&&)                 = default;
    Pet& operator=(Pet&&)      = default;

    void rename(std::string name) { name_ = std::move(name); }

    virtual void make_sound() const {
        std::cout << name_ << " says ...\n";
    }

    [[nodiscard]] virtual std::string_view type() const noexcept { return "Pet"; }
    [[nodiscard]] virtual std::unique_ptr<Pet> clone() const { return std::make_unique<Pet>(*this); }

    virtual void info() const {
        std::cout << "Pet: " << name_ << '\n';
    }

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
};

class Dog : public Pet {
public:
    explicit Dog(std::string name) : Pet(std::move(name)) {}

    void make_sound() const override {
        std::cout << name() << " says woof woof\n";
    }

    [[nodiscard]] std::string_view type() const noexcept override { return "Dog"; }

    [[nodiscard]] std::unique_ptr<Pet> clone() const override {
        return std::make_unique<Dog>(*this);
    }

    void info() const override {
        std::cout << "Dog: " << name() << '\n';
    }
};

class GermanShepherd : public Dog {
public:
    explicit GermanShepherd(std::string name) : Dog(std::move(name)) {}

    void make_sound() const override {
        std::cout << name() << " says wuff wuff\n";
    }

    void laufen() const {
        std::cout << name() << " the German Shepherd is running\n";
    }

    [[nodiscard]] std::string_view type() const noexcept override { return "GermanShepherd"; }

    [[nodiscard]] std::unique_ptr<Pet> clone() const override {
        return std::make_unique<GermanShepherd>(*this);
    }

    void info() const override {
        std::cout << "GermanShepherd: " << name() << '\n';
    }
};

class MastinEspanol : public Dog {
public:
    explicit MastinEspanol(std::string name) : Dog(std::move(name)) {}

    void make_sound() const override {
        std::cout << name() << " says Guau Guau\n";
    }

    void ejecutar() const {
        std::cout << name() << " the Mastin Espanol is running\n";
    }

    [[nodiscard]] std::string_view type() const noexcept override { return "MastinEspanol"; }

    [[nodiscard]] std::unique_ptr<Pet> clone() const override {
        return std::make_unique<MastinEspanol>(*this);
    }

    void info() const override {
        std::cout << "MastinEspanol: " << name() << '\n';
    }
};

void play_with_pet(Pet& pet) {
    if (auto* gs = dynamic_cast<GermanShepherd*>(&pet))
        gs->laufen();
    else if (auto* mes = dynamic_cast<MastinEspanol*>(&pet))
        mes->ejecutar();
    pet.make_sound();
}

void play_with_pet(Pet* pet) {
    if (pet) pet->make_sound();
}

void play_by_value(std::string name, Pet pet) {
    pet.rename(std::move(name));
    pet.make_sound();
}

void play_by_pointer(std::string_view name, Pet* pet) {
    pet->rename(std::string(name));
    pet->make_sound();
}

void play_by_reference(std::string_view name, Pet& pet) {
    pet.rename(std::string(name));
    pet.make_sound();
}

void print_virtual_type(const Pet& pet) {
    std::cout << "Virtual type: " << pet.type() << '\n';
}

[[nodiscard]] std::size_t count_dogs(std::span<Pet* const> pets) {
    return static_cast<std::size_t>(
        std::ranges::count_if(pets, [](Pet* p) { return dynamic_cast<Dog*>(p) != nullptr; }));
}

[[nodiscard]] std::size_t count_german_shepherds(std::span<Pet* const> pets) {
    return static_cast<std::size_t>(
        std::ranges::count_if(pets, [](Pet* p) { return dynamic_cast<GermanShepherd*>(p) != nullptr; }));
}

[[nodiscard]] Pet* find_by_name(std::span<Pet* const> pets, std::string_view target) {
    auto it = std::ranges::find_if(pets, [target](Pet* p) { return p->name() == target; });
    return it != pets.end() ? *it : nullptr;
}

void print_all_info(std::span<Pet* const> pets) {
    std::cout << "\nAll Pet Info:\n";
    std::ranges::for_each(pets, [](const Pet* p) { p->info(); });
}

[[nodiscard]] std::vector<std::unique_ptr<Pet>> clone_all(std::span<Pet* const> pets) {
    std::vector<std::unique_ptr<Pet>> copies;
    copies.reserve(pets.size());
    std::ranges::transform(pets, std::back_inserter(copies),
                           [](const Pet* p) { return p->clone(); });
    return copies;
}

void print_sorted_names(std::span<Pet* const> pets) {
    std::vector<std::string_view> names;
    names.reserve(pets.size());
    std::ranges::transform(pets, std::back_inserter(names), [](const Pet* p) { return p->name(); });
    std::ranges::sort(names);
    std::cout << "\nSorted names:\n";
    std::ranges::for_each(names, [](std::string_view n) { std::cout << n << '\n'; });
}

int main() {
    auto petPtr = std::make_unique<Pet>("Max");

    play_by_pointer("anonymous", petPtr.get());

    Pet petRef("Almonds");
    play_by_reference("anonymous", petRef);
    play_by_pointer("no_name", &petRef);
    play_by_reference("no_name", *petPtr);

    Pet        pet("Animal");
    Dog        dog("Dog");
    GermanShepherd gs("Hund");
    MastinEspanol  mes("Perro");

    play_with_pet(pet);
    play_with_pet(dog);
    play_with_pet(gs);
    play_with_pet(mes);

    play_with_pet(&pet);
    play_with_pet(&dog);
    play_with_pet(static_cast<Pet*>(&gs));
    play_with_pet(static_cast<Pet*>(&mes));

    std::cout << "\n--- Object slicing (Pet copy) ---\n";
    play_by_value("SlicedDog", dog);

    std::cout << "\n--- Virtual type info ---\n";
    print_virtual_type(gs);
    print_virtual_type(mes);

    std::vector<Pet*> pets = { &pet, &dog, &gs, &mes };
    assert(!pets.empty());

    std::cout << "\n--- All pets ---\n";
    std::ranges::for_each(pets, [](const Pet* p) { p->make_sound(); });

    std::cout << "Dog count:             " << count_dogs(pets)             << '\n';
    std::cout << "GermanShepherd count:  " << count_german_shepherds(pets) << '\n';

    std::cout << "\n--- Smart pointer demo ---\n";
    {
        auto smart = std::make_unique<GermanShepherd>("SmartDog");
        smart->make_sound();
    }

    std::cout << "\n--- Clone demo ---\n";
    {
        auto copy = gs.clone();
        copy->make_sound();
    }

    print_all_info(pets);

    std::cout << "\n--- Find by name ---\n";
    if (auto* found = find_by_name(pets, "Hund"))
        std::cout << "Found: " << found->type() << '\n';

    std::cout << "\n--- Cloned pets ---\n";
    auto clones = clone_all(pets);
    std::ranges::for_each(clones, [](const auto& c) { c->make_sound(); });

    print_sorted_names(pets);

    return 0;
}
