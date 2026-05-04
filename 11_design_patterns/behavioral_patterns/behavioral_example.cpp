// Behavioral Patterns Exercise
// Observer, Strategy, Command, State

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm> // tiny addition

// ---------------- Observer ----------------

class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& msg) = 0;
};

class ConcreteObserver : public Observer {
private:
    std::string name;
    
public:
    ConcreteObserver(const std::string& n) : name(n) {}
    
    void update(const std::string& msg) override {
        std::cout << name << " received: " << msg << "\n";
    }
};

class Subject {
private:
    std::vector<std::shared_ptr<Observer>> observers;
    
public:
    void attach(std::shared_ptr<Observer> obs) {
        observers.push_back(obs);
    }

    // ---- small addition ----
    void detach_all() {
        observers.clear();
    }

    std::size_t observer_count() const {
        return observers.size();
    }

    // ---- VERY SMALL NEW ADDITION ----
    void detach_one(const std::shared_ptr<Observer>& obs) {
        observers.erase(
            std::remove(observers.begin(), observers.end(), obs),
            observers.end()
        );
    }
    // --------------------------------
    
    void notify(const std::string& msg) {
        for (auto& obs : observers) {
            obs->update(msg);
        }
    }
};

// ---------------- Strategy ----------------

class Strategy {
public:
    virtual ~Strategy() = default;
    virtual int execute(int a, int b) = 0;
};

class AddStrategy : public Strategy {
public:
    int execute(int a, int b) override { return a + b; }
};

class MultiplyStrategy : public Strategy {
public:
    int execute(int a, int b) override { return a * b; }
};

// Context for Strategy
class Calculator {
private:
    std::shared_ptr<Strategy> strategy;

public:
    void set_strategy(std::shared_ptr<Strategy> s) {
        strategy = s;
    }

    int compute(int a, int b) {
        if (strategy)
            return strategy->execute(a, b);
        return 0;
    }

    // ---- small addition ----
    bool has_strategy() const {
        return strategy != nullptr;
    }

    // ---- VERY SMALL NEW ADDITION ----
    void clear_strategy() {
        strategy.reset();
    }
    // --------------------------------
};

// ---------------- Command ----------------

class Command {
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class PrintCommand : public Command {
private:
    std::string message;

public:
    PrintCommand(const std::string& msg) : message(msg) {}

    void execute() override {
        std::cout << "Command executed: " << message << "\n";
    }
};

// ---- small addition ----
class CommandInvoker {
private:
    std::vector<std::shared_ptr<Command>> history;

public:
    void run(const std::shared_ptr<Command>& cmd) {
        cmd->execute();
        history.push_back(cmd);
    }

    void print_history() const {
        std::cout << "Commands executed: " << history.size() << "\n";
    }

    // ---- VERY SMALL NEW ADDITION ----
    void clear_history() {
        history.clear();
    }
};
// ------------------------

// ---------------- State ----------------

class State {
public:
    virtual ~State() = default;
    virtual void handle() = 0;
};

class IdleState : public State {
public:
    void handle() override {
        std::cout << "System is idle\n";
    }
};

class WorkingState : public State {
public:
    void handle() override {
        std::cout << "System is working\n";
    }
};

class Context {
private:
    std::shared_ptr<State> state;

public:
    void set_state(std::shared_ptr<State> s) {
        state = s;
    }

    void request() {
        if (state)
            state->handle();
    }

    // ---- small addition ----
    bool has_state() const {
        return state != nullptr;
    }

    // ---- VERY SMALL NEW ADDITION ----
    void clear_state() {
        state.reset();
    }
    // --------------------------------
};

// ---------------- Helper (tiny addition) ----------------

void print_divider() {
    std::cout << "------------------------\n";
}

// ---------------- Main ----------------

int main() {

    // Observer demo
    Subject subject;
    auto obs1 = std::make_shared<ConcreteObserver>("Observer1");
    auto obs2 = std::make_shared<ConcreteObserver>("Observer2");

    subject.attach(obs1);
    subject.attach(obs2);
    subject.notify("Event occurred");

    // small extra usage
    std::cout << "Observer count: " << subject.observer_count() << "\n";

    // ---- tiny new usage ----
    subject.detach_one(obs1);
    subject.notify("After removing one observer");

    print_divider();

    // Strategy demo
    Calculator calc;
    calc.set_strategy(std::make_shared<AddStrategy>());
    std::cout << "Add result: " << calc.compute(3, 4) << "\n";

    calc.set_strategy(std::make_shared<MultiplyStrategy>());
    std::cout << "Multiply result: " << calc.compute(3, 4) << "\n";

    std::cout << "Has strategy? "
              << (calc.has_strategy() ? "Yes" : "No") << "\n";

    // ---- tiny new usage ----
    calc.clear_strategy();
    std::cout << "After clear strategy? "
              << (calc.has_strategy() ? "Yes" : "No") << "\n";

    print_divider();

    // Command demo
    CommandInvoker invoker;
    std::shared_ptr<Command> cmd =
        std::make_shared<PrintCommand>("Hello from command pattern");

    invoker.run(cmd);
    invoker.print_history();

    // ---- tiny new usage ----
    invoker.clear_history();
    invoker.print_history();

    print_divider();

    // State demo
    Context context;
    context.set_state(std::make_shared<IdleState>());
    context.request();

    context.set_state(std::make_shared<WorkingState>());
    context.request();

    std::cout << "Has state? "
              << (context.has_state() ? "Yes" : "No") << "\n";

    // ---- tiny new usage ----
    context.clear_state();
    std::cout << "After clear state? "
              << (context.has_state() ? "Yes" : "No") << "\n";

    print_divider();

    // extra: detach observers
    subject.detach_all();
    std::cout << "Observers after clear: "
              << subject.observer_count() << "\n";

    return 0;
}
