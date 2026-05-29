// Graphics and OpenGL Support
// Placeholder for graphics exercises

#include <iostream>

// ✅ ADDED
#include <string>

// ===== VERY SMALL NEW ADDITIONS =====
#include <vector>
#include <iomanip>

// Placeholder function for future graphics demos
void show_topics() {
    std::cout << "\nTopics to explore:\n";
    std::cout << "1. OpenGL initialization (GLFW / GLUT)\n";
    std::cout << "2. Rendering a triangle\n";
    std::cout << "3. Shaders (vertex & fragment)\n";
    std::cout << "4. Transformations (matrices)\n";
    std::cout << "5. Textures and lighting\n";
}

// ✅ ADDED: Simulated modules

void basic_rendering_demo() {
    std::cout << "\n[Basic Rendering]\n";
    std::cout << "-> Initialize window\n";
    std::cout << "-> Setup viewport\n";
    std::cout << "-> Draw triangle (concept)\n";
}

void shader_demo() {
    std::cout << "\n[Shader Experiment]\n";
    std::cout << "-> Vertex Shader: processes vertices\n";
    std::cout << "-> Fragment Shader: colors pixels\n";
}

void transformation_demo() {
    std::cout << "\n[3D Transformations]\n";
    std::cout << "-> Translation, Rotation, Scaling\n";
    std::cout << "-> Uses matrices (model, view, projection)\n";
}

void lighting_demo() {
    std::cout << "\n[Lighting Models]\n";
    std::cout << "-> Ambient, Diffuse, Specular\n";
    std::cout << "-> Phong / Blinn-Phong models\n";
}

// ---------------- NEW SMALL ADDITIONS ----------------

// input validation
int get_valid_choice() {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 4) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. Enter a number (1-4): ";
        } else {
            return choice;
        }
    }
}

// repeat option
bool ask_repeat() {
    char ans;
    std::cout << "\nRun another demo? (y/n): ";
    std::cin >> ans;
    return (ans == 'y' || ans == 'Y');
}

// ✅ ADDED: simple header printer
void print_header(const std::string& title) {
    std::cout << "\n==============================\n";
    std::cout << title << "\n";
    std::cout << "==============================\n";
}

// ✅ ADDED: fake clear screen (portable)
void clear_screen() {
    std::cout << std::string(40, '\n');
}

// -----------------------------------------

// ===== VERY SMALL EXTRA HELPERS =====

// Demo history tracker
void show_history(const std::vector<std::string>& history) {
    std::cout << "\nDemo History:\n";

    if (history.empty()) {
        std::cout << "No demos executed yet\n";
        return;
    }

    for (std::size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << ". " << history[i] << "\n";
    }
}

// Fake FPS counter
void simulate_fps() {
    std::cout << "Simulated FPS: "
              << std::fixed << std::setprecision(1)
              << 60.0 << "\n";
}

// Small OpenGL tips
void graphics_tip() {
    std::cout << "\nGraphics Tip:\n";
    std::cout << "- Modern OpenGL uses shaders instead of fixed pipeline\n";
}

// =====================================

// ✅ ADDED: Menu system
void run_demo(int choice) {
    switch (choice) {
        case 1: basic_rendering_demo(); break;
        case 2: shader_demo(); break;
        case 3: transformation_demo(); break;
        case 4: lighting_demo(); break;
        default: std::cout << "Invalid choice\n";
    }
}

int main() {

    std::cout << "Graphics and OpenGL learning area\n";

    std::cout << "\nAvailable experiments:\n";
    std::cout << "1. Basic rendering\n";
    std::cout << "2. Shader experiments\n";
    std::cout << "3. 3D transformations\n";
    std::cout << "4. Lighting models\n";

    show_topics();

    // ✅ ADDED: track how many demos run
    int demo_count = 0;

    // ===== VERY SMALL NEW ADDITIONS =====
    std::vector<std::string> history;
    // ====================================

    // ✅ UPDATED: looped interaction (non-breaking addition)
    do {
        print_header("OpenGL Demo Menu");

        std::cout << "Select an experiment (1-4): ";
        int choice = get_valid_choice();

        clear_screen(); // cleaner output
        run_demo(choice);

        // ===== VERY SMALL EXTRA USAGE =====

        switch (choice) {
            case 1: history.push_back("Basic Rendering"); break;
            case 2: history.push_back("Shader Experiment"); break;
            case 3: history.push_back("3D Transformations"); break;
            case 4: history.push_back("Lighting Models"); break;
        }

        simulate_fps();
        graphics_tip();

        // ==================================

        ++demo_count;

    } while (ask_repeat());

    // ✅ ADDED: usage summary
    std::cout << "\nTotal demos run: " << demo_count << "\n";

    // ===== VERY SMALL EXTRA OUTPUT =====
    show_history(history);
    // ===================================

    // ✅ ADDED: future roadmap
    std::cout << "\nUpcoming upgrades:\n";
    std::cout << "- GLFW window creation\n";
    std::cout << "- GLEW/GLAD setup\n";
    std::cout << "- Real triangle rendering with shaders\n";

    // ✅ ADDED: Future hint
    std::cout << "\n(Next step: integrate real OpenGL libraries like GLFW + GLEW)\n";

    return 0;
}
