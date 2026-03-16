// Rendering Techniques
// 3D rendering concepts

#include <iostream>

// Helper function to show rendering topics
void show_rendering_topics() {
    std::cout << "\nCommon Rendering Techniques:\n";
    std::cout << "1. Rasterization (standard GPU pipeline)\n";
    std::cout << "2. Ray Tracing (realistic lighting simulation)\n";
    std::cout << "3. Shadow Mapping\n";
    std::cout << "4. Deferred Rendering\n";
    std::cout << "5. Physically Based Rendering (PBR)\n";
}

int main() {

    std::cout << "Rendering techniques area\n";

    std::cout << "\nBasic 3D Graphics Pipeline:\n";
    std::cout << "Vertex Processing -> Primitive Assembly -> Rasterization -> Fragment Processing\n";

    show_rendering_topics();

    return 0;
}
