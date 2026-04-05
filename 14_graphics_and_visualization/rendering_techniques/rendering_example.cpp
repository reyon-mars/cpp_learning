// Rendering Techniques
// 3D rendering concepts

#include <iostream>

// ✅ ADDED
#include <string>

// Helper function to show rendering topics
void show_rendering_topics() {
    std::cout << "\nCommon Rendering Techniques:\n";
    std::cout << "1. Rasterization (standard GPU pipeline)\n";
    std::cout << "2. Ray Tracing (realistic lighting simulation)\n";
    std::cout << "3. Shadow Mapping\n";
    std::cout << "4. Deferred Rendering\n";
    std::cout << "5. Physically Based Rendering (PBR)\n";
}

// ✅ ADDED: Detailed explanations

void rasterization_demo() {
    std::cout << "\n[Rasterization]\n";
    std::cout << "Fast rendering using triangles\n";
    std::cout << "Converts 3D objects into pixels\n";
}

void ray_tracing_demo() {
    std::cout << "\n[Ray Tracing]\n";
    std::cout << "Simulates light rays\n";
    std::cout << "Produces realistic reflections and shadows\n";
}

void shadow_mapping_demo() {
    std::cout << "\n[Shadow Mapping]\n";
    std::cout << "Renders scene from light's perspective\n";
    std::cout << "Determines shadowed areas\n";
}

void deferred_rendering_demo() {
    std::cout << "\n[Deferred Rendering]\n";
    std::cout << "Stores geometry info in buffers (G-buffer)\n";
    std::cout << "Applies lighting later for efficiency\n";
}

void pbr_demo() {
    std::cout << "\n[Physically Based Rendering]\n";
    std::cout << "Uses realistic material properties\n";
    std::cout << "Simulates real-world lighting behavior\n";
}

// ✅ ADDED: Pipeline explanation
void pipeline_breakdown() {
    std::cout << "\nPipeline Breakdown:\n";
    std::cout << "1. Vertex Processing: Transform vertices\n";
    std::cout << "2. Primitive Assembly: Form triangles\n";
    std::cout << "3. Rasterization: Convert to fragments\n";
    std::cout << "4. Fragment Processing: Apply color & lighting\n";
}

// ✅ ADDED: Menu system
void run_demo(int choice) {
    switch (choice) {
        case 1: rasterization_demo(); break;
        case 2: ray_tracing_demo(); break;
        case 3: shadow_mapping_demo(); break;
        case 4: deferred_rendering_demo(); break;
        case 5: pbr_demo(); break;
        default: std::cout << "Invalid choice\n";
    }
}

int main() {

    std::cout << "Rendering techniques area\n";

    std::cout << "\nBasic 3D Graphics Pipeline:\n";
    std::cout << "Vertex Processing -> Primitive Assembly -> Rasterization -> Fragment Processing\n";

    show_rendering_topics();

    // ✅ ADDED
    pipeline_breakdown();

    // ✅ ADDED: User interaction
    std::cout << "\nSelect a technique to explore (1-5): ";
    int choice;
    std::cin >> choice;

    run_demo(choice);

    // ✅ ADDED: Summary tip
    std::cout << "\nTip: Rasterization is fast, Ray Tracing is realistic.\n";

    return 0;
}
