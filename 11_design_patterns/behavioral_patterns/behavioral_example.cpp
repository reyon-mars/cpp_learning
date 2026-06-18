#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <string_view>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <span>
#include <concepts>
#include <functional>
#include <optional>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(std::string_view msg) = 0;
};

class ConcreteObserver : public Observer {
    std::string name_;
public:
    explicit ConcreteObserver(std::string name) : name_(std::move(name)) {}

    void update(std::string_view msg) override {
        std::cout << name_ << " received: " << msg << '\n';
    }

    [[nodiscard]] const std::string& name() const noexcept { return name_; }
};

class Subject {
    std::vector<std::shared_ptr<Observer>> observers_;

public:
    void attach(std::shared_ptr<Observer> obs) {
        observers_.push_back(std::move(obs));
    }

    void detach(const std::shared_ptr<Observer>& obs) {
        std::erase(observers_, obs);
    }

    void detach_all() noexcept { observers_.clear(); }

    void notify(std::string_view msg) const {
        std::ranges::for_each(observers_, [&](const auto& obs) { obs->update(msg); });
    }

    [[nodiscard]] std::size_t observer_count() const noexcept { return observers_.size(); }
    [[nodiscard]] bool empty()                 const noexcept { return observers_.empty(); }
};

class Strategy {
public:
    virtual ~Strategy() = default;
    [[nodiscard]] virtual int execute(int a, int b) const = 0;
};

class AddStrategy : public Strategy {
public:
    [[nodiscard]] int execute(int a, int b) const override { return a + b; }
};

class MultiplyStrategy : public Strategy {
public:
    [[nodiscard]] int execute(int a, int b) const override { return a * b; }
};

class SubtractStrategy : public Strategy {
public:
    [[nodiscard]] int execute(int a, int b) const override { return a - b; }
};

class Calculator {
    std::shared_ptr<Strategy> strategy_;

public:
    void set_strategy(std::shared_ptr<Strategy> s) noexcept { strategy_ = std::move(s); }
    void clear_strategy() noexcept { strategy_.reset(); }

    [[nodiscard]] std::optional<int> compute(int a, int b) const {
        if (!strategy_) return std::nullopt;
        return strategy_->execute(a, b);
    }

    [[nodiscard]] bool has_strategy() const noexcept { return strategy_ != nullptr; }
};

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class PrintCommand : public Command {
    std::string message_;
public:
    explicit PrintCommand(std::string msg) : message_(std::move(msg)) {}

    void execute() override {
        std::cout << "Command executed: " << message_ << '\n';
    }
};

class CommandInvoker {
    std::vector<std::shared_ptr<Command>> history_;

public:
    void run(const std::shared_ptr<Command>& cmd) {
        cmd->execute();
        history_.push_back(cmd);
    }

    void clear_history() noexcept { history_.clear(); }

    void print_history() const {
        std::cout << "Commands executed: " << history_.size() << '\n';
    }

    [[nodiscard]] std::size_t history_size() const noexcept { return history_.size(); }
};

class State {
public:
    virtual ~State() = default;
    virtual void handle() const = 0;
};

class IdleState : public State {
public:
    void handle() const override { std::cout << "System is idle\n"; }
};

class WorkingState : public State {
public:
    void handle() const override { std::cout << "System is working\n"; }
};

class ErrorState : public State {
public:
    void handle() const override { std::cout << "System error state\n"; }
};

class Context {
    std::shared_ptr<State> state_;

public:
    void set_state(std::shared_ptr<State> s) noexcept { state_ = std::move(s); }
    void clear_state() noexcept { state_.reset(); }

    void request() const {
        if (state_) state_->handle();
    }

    [[nodiscard]] bool has_state() const noexcept { return state_ != nullptr; }
};

void print_divider() {
    std::cout << "------------------------\n";
}

void print_title(std::string_view title) {
    std::cout << "\n=== " << title << " ===\n";
}

int main() {
    print_title("Observer Pattern");

    Subject subject;
    auto obs1 = std::make_shared<ConcreteObserver>("Observer1");
    auto obs2 = std::make_shared<ConcreteObserver>("Observer2");

    subject.attach(obs1);
    subject.attach(obs2);
    subject.notify("Event occurred");

    std::cout << "Observer count: " << subject.observer_count() << '\n';

    subject.detach(obs1);
    subject.notify("After removing one observer");

    print_divider();

    print_title("Strategy Pattern");

    Calculator calc;
    calc.set_strategy(std::make_shared<AddStrategy>());
    std::cout << "Add result: " << calc.compute(3, 4).value_or(0) << '\n';

    calc.set_strategy(std::make_shared<MultiplyStrategy>());
    std::cout << "Multiply result: " << calc.compute(3, 4).value_or(0) << '\n';

    calc.set_strategy(std::make_shared<SubtractStrategy>());
    std::cout << "Subtract result: " << calc.compute(10, 4).value_or(0) << '\n';

    std::cout << "Has strategy? " << (calc.has_strategy() ? "Yes" : "No") << '\n';

    calc.clear_strategy();
    std::cout << "After clear strategy? " << (calc.has_strategy() ? "Yes" : "No") << '\n';

    print_divider();

    print_title("Command Pattern");

    CommandInvoker invoker;
    auto cmd = std::make_shared<PrintCommand>("Hello from command pattern");

    invoker.run(cmd);
    invoker.print_history();

    std::cout << "History size: " << invoker.history_size() << '\n';

    invoker.clear_history();
    invoker.print_history();

    print_divider();

    print_title("State Pattern");

    Context context;
    context.set_state(std::make_shared<IdleState>());
    context.request();

    context.set_state(std::make_shared<WorkingState>());
    context.request();

    context.set_state(std::make_shared<ErrorState>());
    context.request();

    std::cout << "Has state? " << (context.has_state() ? "Yes" : "No") << '\n';

    context.clear_state();
    std::cout << "After clear state? " << (context.has_state() ? "Yes" : "No") << '\n';

    print_divider();

    subject.detach_all();
    std::cout << "Observers after clear: " << subject.observer_count() << '\n';

    const std::vector<int> demo_values = {1, 2, 3, 4};
    std::cout << "Accumulated value: "
              << std::accumulate(demo_values.begin(), demo_values.end(), 0) << '\n';

    std::cout << "Subject empty? " << (subject.empty() ? "Yes" : "No") << '\n';

    return 0;
}
