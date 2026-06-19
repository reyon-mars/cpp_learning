// Shader Programming
// GLSL and shader implementations

#include <iostream>


#include <string>

#include <vector>
#include <iomanip>

// Example GLSL shaders as strings
void show_example_shaders() {

    const char* vertex_shader = R"(

    #version 330 core
    layout (location = 0) in vec3 aPos;

    void main()
    {
        gl_Position = vec4(aPos, 1.0);
    }

    )";

    const char* fragment_shader = R"(

    #version 330 core
    out vec4 FragColor;

    void main()
    {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }

    )";

    std::cout << "\nExample Vertex Shader:\n";
    std::cout << vertex_shader << "\n";

    std::cout << "\nExample Fragment Shader:\n";
    std::cout << fragment_shader << "\n";
}

: Uniform example
void uniform_shader_example() {
    const char* shader = R"(

    #version 330 core
    out vec4 FragColor;
    uniform float time;

    void main()
    {
        float green = (sin(time) + 1.0) / 2.0;
        FragColor = vec4(0.0, green, 0.0, 1.0);
    }

    )";

    std::cout << "\n[Uniform Shader Example]\n";
    std::cout << shader << "\n";
}

: Simple lighting shader
void lighting_shader_example() {
    const char* shader = R"(

    #version 330 core
    out vec4 FragColor;

    void main()
    {
        float intensity = 0.8;
        FragColor = vec4(intensity, intensity, intensity, 1.0);
    }

    )";

    std::cout << "\n[Basic Lighting Shader]\n";
    std::cout << shader << "\n";
}

: Shader explanations
void explain_shader(int choice) {
    switch (choice) {
        case 1:
            std::cout << "\nVertex Shader: Processes vertex positions\n";
            break;
        case 2:
            std::cout << "\nFragment Shader: Determines pixel color\n";
            break;
        case 3:
            std::cout << "\nGeometry Shader: Modifies primitives\n";
            break;
        case 4:
            std::cout << "\nCompute Shader: General GPU computation\n";
            break;
        default:
            std::cout << "Invalid choice\n";
    }
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
            std::cout << "Invalid input. Enter (1-4): ";
        } else {
            return choice;
        }
    }
}

// repeat option
bool ask_repeat() {
    char ans;
    std::cout << "\nExplore another shader? (y/n): ";
    std::cin >> ans;
    return (ans == 'y' || ans == 'Y');
}

// pipeline stages overview
void pipeline_overview() {
    std::cout << "\nGPU Pipeline Stages:\n";
    std::cout << "- Vertex Shader\n";
    std::cout << "- Geometry Shader (optional)\n";
    std::cout << "- Rasterization\n";
    std::cout << "- Fragment Shader\n";
}

// -----------------------------------------------------

// ===== VERY SMALL EXTRA HELPERS =====

// pretty section header
void print_header(const std::string& title) {
    std::cout << "\n=================================\n";
    std::cout << title << "\n";
    std::cout << "=================================\n";
}

// show shader difficulty
void shader_difficulty(int choice) {

    std::cout << "Difficulty: ";

    switch (choice) {
        case 1:
            std::cout << "Beginner\n";
            break;
        case 2:
            std::cout << "Beginner\n";
            break;
        case 3:
            std::cout << "Intermediate\n";
            break;
        case 4:
            std::cout << "Advanced\n";
            break;
        default:
            std::cout << "Unknown\n";
    }
}

// explored history
void show_history(const std::vector<std::string>& history) {

    std::cout << "\nExplored Shader Types:\n";

    if (history.empty()) {
        std::cout << "None\n";
        return;
    }

    for (std::size_t i = 0; i < history.size(); ++i) {
        std::cout << std::setw(2) << i + 1
                  << ". " << history[i] << "\n";
    }
}

// ====================================

int main() {

    std::cout << "Shader programming area\n";

    std::cout << "\nShader Types:\n";
    std::cout << "1. Vertex Shader\n";
    std::cout << "2. Fragment Shader\n";
    std::cout << "3. Geometry Shader\n";
    std::cout << "4. Compute Shader\n";

    show_example_shaders();

    
    pipeline_overview();

        int explored_count = 0;
    std::vector<std::string> history;
    // ====================================

    // ✅ UPDATED: loop interaction
    do {

        print_header("Shader Learning Menu");

        std::cout << "\nSelect shader type to learn more (1-4): ";
        int choice = get_valid_choice();

        explain_shader(choice);

        // ===== VERY SMALL EXTRA USAGE =====

        shader_difficulty(choice);

        switch (choice) {
            case 1: history.push_back("Vertex Shader"); break;
            case 2: history.push_back("Fragment Shader"); break;
            case 3: history.push_back("Geometry Shader"); break;
            case 4: history.push_back("Compute Shader"); break;
        }

        ++explored_count;

        // ==================================

    } while (ask_repeat());

    : Show advanced examples
    uniform_shader_example();
    lighting_shader_example();

    // ===== VERY SMALL EXTRA OUTPUT =====
    std::cout << "\nTotal shader topics explored: "
              << explored_count << "\n";

    show_history(history);
    // ===================================

    : Summary
    std::cout << "\nTip: Shaders run on GPU and control rendering pipeline stages.\n";

    // ===== VERY SMALL EXTRA TIP =====
    std::cout << "Modern graphics engines rely heavily on optimized GLSL shaders.\n";
    // =================================

    return 0;
}
