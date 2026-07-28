#include <iostream>
#include <type_traits>
#include <concepts>
#include <cassert>
#include <format>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cstddef>

template<typename Derived>
class Base {
public:
    void interface() {
        pre();
        self().implementation();
        post();
    }

    void common() const {
        std::cout << "Common CRTP behavior\n";
    }

    void call_twice() {
        std::cout << "[Base] Calling implementation twice:\n";
        self().implementation();
        self().implementation();
    }

    void type_info() const {
        std::cout << std::format("CRTP Derived size: {} bytes\n", sizeof(Derived));
    }

    void static_dispatch() {
        std::cout << "[Base] Static dispatch: ";
        self().implementation();
    }

    [[nodiscard]] bool is_same_type(const Base&) const noexcept { return true; }

protected:
    ~Base() = default;

private:
    [[nodiscard]] Derived&       self()       noexcept { return *static_cast<Derived*>(this);       }
    [[nodiscard]] const Derived& self() const noexcept { return *static_cast<const Derived*>(this); }

    void pre()  const { std::cout << "[Base] Before implementation\n"; }
    void post() const { std::cout << "[Base] After implementation\n";  }
};

template<typename T>
concept CRTPDerived = requires(T obj) {
    { obj.implementation() };
    { obj.extra_feature()  };
    { obj.unique_task()    };
};

template<typename Derived>
class Countable : public Base<Derived> {
    inline static int instance_count_ = 0;
public:
    Countable()  noexcept { ++instance_count_; }
    ~Countable()          { --instance_count_; }
    Countable(const Countable&)            noexcept { ++instance_count_; }
    Countable& operator=(const Countable&) = default;

    [[nodiscard]] static int instance_count() noexcept { return instance_count_; }
};

template<typename Derived>
class Accumulator : public Base<Derived> {
    int total_ = 0;
public:
    void accumulate(int v) { total_ += v; }
    [[nodiscard]] int total() const noexcept { return total_; }
    void reset() noexcept { total_ = 0; }
};

class Derived : public Countable<Derived> {
public:
    void implementation() const { std::cout << "Derived::implementation\n";  }
    void extra_feature()  const { std::cout << "Derived::extra_feature\n";   }
    void unique_task()    const { std::cout << "Derived::unique_task\n";      }
};

class AnotherDerived : public Countable<AnotherDerived> {
public:
    void implementation() const { std::cout << "AnotherDerived::implementation\n"; }
    void extra_feature()  const { std::cout << "AnotherDerived::extra_feature\n";  }
    void unique_task()    const { std::cout << "AnotherDerived::unique_task\n";     }
};

class SumDerived : public Accumulator<SumDerived> {
    std::string name_;
public:
    explicit SumDerived(std::string name) : name_(std::move(name)) {}
    void implementation() const { std::cout << std::format("SumDerived({})::implementation\n", name_); }
    void extra_feature()  const { std::cout << std::format("SumDerived({}) total={}\n", name_, total()); }
    void unique_task()    const { std::cout << std::format("SumDerived({})::unique_task\n", name_); }
};

template<typename T>
void run_crtp(Base<T>& obj) {
    std::cout << "[Generic] run_crtp:\n";
    obj.interface();
}

template<typename T>
void check_crtp() {
    std::cout << std::format("is_class={}\n", std::is_class_v<T>);
    static_assert(std::is_base_of_v<Base<T>, T>, "T must publicly inherit Base<T>");
}

template<CRTPDerived T>
void execute_extra(const T& obj) {
    std::cout << "[Generic] extra_feature: ";
    obj.extra_feature();
}

template<CRTPDerived T>
void run_all(T& obj) {
    obj.interface();
    obj.common();
    obj.call_twice();
    obj.extra_feature();
    obj.type_info();
    obj.static_dispatch();
    obj.unique_task();
}

template<CRTPDerived T>
void benchmark_calls(T& obj, int n) {
    std::cout << std::format("[Benchmark] {} calls to implementation:\n", n);
    for (int i = 0; i < n; ++i) obj.implementation();
}

int main() {
    Derived       d;
    AnotherDerived ad;

    std::cout << "=== Derived ===\n";
    run_all(d);

    std::cout << "\n=== AnotherDerived ===\n";
    run_all(ad);

    std::cout << "\n=== Generic run_crtp ===\n";
    run_crtp(d);
    run_crtp(ad);

    std::cout << "\n=== check_crtp ===\n";
    check_crtp<Derived>();
    check_crtp<AnotherDerived>();

    std::cout << "\n=== execute_extra ===\n";
    execute_extra(d);
    execute_extra(ad);

    std::cout << "\n=== Countable: instance_count ===\n";
    std::cout << std::format("Derived instances: {}\n",        Derived::instance_count());
    std::cout << std::format("AnotherDerived instances: {}\n", AnotherDerived::instance_count());
    {
        Derived d2;
        Derived d3;
        std::cout << std::format("After creating 2 more Derived: {}\n", Derived::instance_count());
    }
    std::cout << std::format("After scope exit: {}\n", Derived::instance_count());

    std::cout << "\n=== Accumulator: SumDerived ===\n";
    SumDerived sd("alpha");
    sd.accumulate(10);
    sd.accumulate(20);
    sd.accumulate(30);
    sd.extra_feature();
    sd.implementation();
    sd.interface();
    sd.reset();
    sd.extra_feature();

    std::cout << "\n=== benchmark_calls ===\n";
    benchmark_calls(d, 3);

    std::cout << "\n=== static_assert checks ===\n";
    static_assert(std::is_base_of_v<Base<Derived>,        Derived>);
    static_assert(std::is_base_of_v<Base<AnotherDerived>, AnotherDerived>);
    static_assert(CRTPDerived<Derived>);
    static_assert(CRTPDerived<AnotherDerived>);
    static_assert(CRTPDerived<SumDerived>);

    assert(Derived::instance_count() == 1);
    assert(AnotherDerived::instance_count() == 1);

    std::cout << "\nAll assertions passed.\n";
    return 0;
}
