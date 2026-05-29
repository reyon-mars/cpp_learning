// Rendering Techniques
// 3D rendering concepts

#include <iostream>

// ✅ ADDED
#include <string>

// ===== VERY SMALL NEW ADDITIONS =====
#include <vector>
#include <iomanip>

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

// ---------------- NEW SMALL ADDITIONS ----------------

// input validation
int get_valid_choice() {
    int choice;
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > 5) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. Enter (1-5): ";
        } else {
            return choice;
        }
    }
}

// repeat option
bool ask_repeat() {
    char ans;
    std::cout << "\nExplore another technique? (y/n): ";
    std::cin >> ans;
    return (ans == 'y' || ans == 'Y');
}

// comparison helper
void compare_modes() {
    std::cout << "\nQuick Comparison:\n";
    std::cout << "- Rasterization: Very fast, less realistic\n";
    std::cout << "- Ray Tracing: Slower, highly realistic\n";
}

// -----------------------------------------------------

// ===== VERY SMALL EXTRA HELPERS =====

// print section title
void print_header(const std::string& title) {
    std::cout << "\n================================\n";
    std::cout << title << "\n";
    std::cout << "================================\n";
}

// fake GPU stats
void show_gpu_stats() {
    std::cout << "\n[GPU Stats Simulation]\n";
    std::cout << "VRAM Usage: " << std::fixed << std::setprecision(1)
              << 2.5 << " GB\n";
    std::cout << "GPU Load: 78%\n";
}

// show explored history
void show_history(const std::vector<std::string>& history) {
    std::cout << "\nExplored Techniques:\n";

    if (history.empty()) {
        std::cout << "No techniques explored\n";
        return;
    }

    for (std::size_t i = 0; i < history.size(); ++i) {
        std::cout << i + 1 << ". " << history[i] << "\n";
    }
}

// ====================================

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

    // ===== VERY SMALL NEW ADDITIONS =====
    int explored_count = 0;
    std::vector<std::string> history;
    // ====================================

    // ✅ UPDATED: loop interaction
    do {

        print_header("Rendering Technique Explorer");

        std::cout << "\nSelect a technique to explore (1-5): ";
        int choice = get_valid_choice();

        run_demo(choice);

        // ===== VERY SMALL EXTRA USAGE =====

        switch (choice) {
            case 1: history.push_back("Rasterization"); break;
            case 2: history.push_back("Ray Tracing"); break;
            case 3: history.push_back("Shadow Mapping"); break;
            case 4: history.push_back("Deferred Rendering"); break;
            case 5: history.push_back("Physically Based Rendering"); break;
        }

        ++explored_count;

        show_gpu_stats();

        // ==================================

    } while (ask_repeat());

    // ✅ ADDED
    compare_modes();

    // ===== VERY SMALL EXTRA OUTPUT =====
    std::cout << "\nTotal techniques explored: "
              << explored_count << "\n";

    show_history(history);
    // ===================================

    // ✅ ADDED: Summary tip
    std::cout << "\nTip: Rasterization is fast, Ray Tracing is realistic.\n";

    // ===== VERY SMALL EXTRA TIP =====
    std::cout << "Modern game engines often combine multiple rendering techniques.\n";
    // =================================

    return 0;
}
