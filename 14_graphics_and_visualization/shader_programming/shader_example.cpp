// Shader Programming
// GLSL and shader implementations

#include <iostream>

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

int main() {

    std::cout << "Shader programming area\n";

    std::cout << "\nShader Types:\n";
    std::cout << "1. Vertex Shader\n";
    std::cout << "2. Fragment Shader\n";
    std::cout << "3. Geometry Shader\n";
    std::cout << "4. Compute Shader\n";

    show_example_shaders();

    return 0;
}
