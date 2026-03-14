// Behavioral Patterns Exercise
// Observer, Strategy, Command, State

#include <iostream>
#include <vector>
#include <memory>
#include <string>

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
};

// ---------------- Main ----------------

int main() {

    // Observer demo
    Subject subject;
    subject.attach(std::make_shared<ConcreteObserver>("Observer1"));
    subject.attach(std::make_shared<ConcreteObserver>("Observer2"));
    subject.notify("Event occurred");

    std::cout << "\n";

    // Strategy demo
    Calculator calc;
    calc.set_strategy(std::make_shared<AddStrategy>());
    std::cout << "Add result: " << calc.compute(3, 4) << "\n";

    calc.set_strategy(std::make_shared<MultiplyStrategy>());
    std::cout << "Multiply result: " << calc.compute(3, 4) << "\n";

    std::cout << "\n";

    // Command demo
    std::shared_ptr<Command> cmd =
        std::make_shared<PrintCommand>("Hello from command pattern");
    cmd->execute();

    std::cout << "\n";

    // State demo
    Context context;
    context.set_state(std::make_shared<IdleState>());
    context.request();

    context.set_state(std::make_shared<WorkingState>());
    context.request();

    return 0;
}
