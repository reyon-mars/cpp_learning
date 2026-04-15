// Shader Programming
// GLSL and shader implementations

#include <iostream>

// ✅ ADDED
#include <string>

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

// ✅ ADDED: Uniform example
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

// ✅ ADDED: Simple lighting shader
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

// ✅ ADDED: Shader explanations
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

int main() {

    std::cout << "Shader programming area\n";

    std::cout << "\nShader Types:\n";
    std::cout << "1. Vertex Shader\n";
    std::cout << "2. Fragment Shader\n";
    std::cout << "3. Geometry Shader\n";
    std::cout << "4. Compute Shader\n";

    show_example_shaders();

    // ✅ ADDED
    pipeline_overview();

    // ✅ UPDATED: loop interaction
    do {
        std::cout << "\nSelect shader type to learn more (1-4): ";
        int choice = get_valid_choice();

        explain_shader(choice);

    } while (ask_repeat());

    // ✅ ADDED: Show advanced examples
    uniform_shader_example();
    lighting_shader_example();

    // ✅ ADDED: Summary
    std::cout << "\nTip: Shaders run on GPU and control rendering pipeline stages.\n";

    return 0;
}
