#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <iomanip>

void basic_rendering_demo() {
    std::cout << "[Basic Rendering]\n"
              << "  -> Initialise window\n"
              << "  -> Setup viewport\n"
              << "  -> Draw triangle (concept)\n";
}

void shader_demo() {
    std::cout << "[Shader Experiment]\n"
              << "  -> Vertex Shader: processes vertices\n"
              << "  -> Fragment Shader: colours pixels\n";
}

void transformation_demo() {
    std::cout << "[3D Transformations]\n"
              << "  -> Translation, Rotation, Scaling\n"
              << "  -> Model / View / Projection matrices\n";
}

void lighting_demo() {
    std::cout << "[Lighting Models]\n"
              << "  -> Ambient, Diffuse, Specular\n"
              << "  -> Phong / Blinn-Phong\n";
}

void run_demo(int choice) {
    switch (choice) {
        case 1: basic_rendering_demo(); break;
        case 2: shader_demo();          break;
        case 3: transformation_demo();  break;
        case 4: lighting_demo();        break;
        default: std::cout << "Invalid choice\n";
    }
}

void print_header(std::string_view title) {
    std::cout << "\n==============================\n"
              << title << "\n"
              << "==============================\n";
}

[[nodiscard]] int get_valid_choice() {
    int choice{};
    while (true) {
        std::cin >> choice;
        if (!std::cin.fail() && choice >= 1 && choice <= 4) return choice;
        std::cin.clear();
        std::cin.ignore(1000, '\n');
        std::cout << "Enter 1–4: ";
    }
}

[[nodiscard]] bool ask_repeat() {
    char ans{};
    std::cout << "\nRun another demo? (y/n): ";
    std::cin >> ans;
    return ans == 'y' || ans == 'Y';
}

void show_history(const std::vector<std::string>& history) {
    std::cout << "\nDemo History:\n";
    if (history.empty()) { std::cout << "  (none)\n"; return; }
    for (std::size_t i = 0; i < history.size(); ++i)
        std::cout << "  " << i + 1 << ". " << history[i] << "\n";
}

void graphics_tip() {
    std::cout << "Tip: Modern OpenGL uses programmable shaders, not a fixed pipeline.\n";
}

constexpr std::string_view DEMO_NAMES[] = {
    "", "Basic Rendering", "Shader Experiment",
    "3D Transformations", "Lighting Models"
};

int main() {
    std::cout << "Graphics / OpenGL learning area\n\n"
              << "Topics to explore:\n"
              << "  1. OpenGL initialisation (GLFW / GLUT)\n"
              << "  2. Rendering a triangle\n"
              << "  3. Shaders (vertex & fragment)\n"
              << "  4. Transformations (matrices)\n"
              << "  5. Textures and lighting\n";

    int demo_count = 0;
    std::vector<std::string> history;

    do {
        print_header("OpenGL Demo Menu");
        std::cout << "  1. Basic rendering\n"
                  << "  2. Shader experiments\n"
                  << "  3. 3D transformations\n"
                  << "  4. Lighting models\n"
                  << "Select (1-4): ";

        const int choice = get_valid_choice();
        run_demo(choice);
        history.emplace_back(DEMO_NAMES[choice]);
        graphics_tip();
        std::cout << "Simulated FPS: "
                  << std::fixed << std::setprecision(1) << 60.0 << "\n";
        ++demo_count;

    } while (ask_repeat());

    std::cout << "\nTotal demos run: " << demo_count << "\n";
    show_history(history);

    std::cout << "\nUpcoming steps:\n"
              << "  - GLFW window creation\n"
              << "  - GLEW / GLAD setup\n"
              << "  - Real triangle rendering with shaders\n"
              << "\n(Next: integrate GLFW + GLEW for real OpenGL)\n";

    return 0;
}
