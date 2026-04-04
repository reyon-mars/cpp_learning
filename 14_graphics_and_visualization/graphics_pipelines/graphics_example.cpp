// Graphics and OpenGL Support
// Placeholder for graphics exercises

#include <iostream>

// ✅ ADDED
#include <string>

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

    // ✅ ADDED: User interaction
    std::cout << "\nSelect an experiment (1-4): ";
    int choice;
    std::cin >> choice;

    run_demo(choice);

    // ✅ ADDED: Future hint
    std::cout << "\n(Next step: integrate real OpenGL libraries like GLFW + GLEW)\n";

    return 0;
}
