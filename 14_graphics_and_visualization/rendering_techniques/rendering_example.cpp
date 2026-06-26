#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <iomanip>
#include <format>
#include <array>
#include <optional>
#include <functional>
#include <ranges>
#include <span>

enum class Technique {
    Rasterization = 1,
    RayTracing,
    ShadowMapping,
    DeferredRendering,
    PBR,
};

struct TechniqueInfo {
    Technique tech;
    std::string_view name;
    std::string_view header;
    std::array<std::string_view, 2> lines;
};

inline constexpr std::array<TechniqueInfo, 5> kTechniques{{
    {Technique::Rasterization, "Rasterization", "Rasterization",
     {"Fast rendering using triangles", "Converts 3D objects into pixels"}},
    {Technique::RayTracing, "Ray Tracing", "Ray Tracing",
     {"Simulates light rays", "Produces realistic reflections and shadows"}},
    {Technique::ShadowMapping, "Shadow Mapping", "Shadow Mapping",
     {"Renders scene from light's perspective", "Determines shadowed areas"}},
    {Technique::DeferredRendering, "Deferred Rendering", "Deferred Rendering",
     {"Stores geometry info in buffers (G-buffer)", "Applies lighting later for efficiency"}},
    {Technique::PBR, "Physically Based Rendering", "Physically Based Rendering",
     {"Uses realistic material properties", "Simulates real-world lighting behavior"}},
}};

void show_rendering_topics() {
    std::cout << "\nCommon Rendering Techniques:\n";
    for (std::size_t i = 0; i < kTechniques.size(); ++i)
        std::cout << std::format("{}. {}\n", i + 1, kTechniques[i].name);
}

void pipeline_breakdown() {
    std::cout << "\nPipeline Breakdown:\n";
    constexpr std::array stages{
        "Vertex Processing: Transform vertices",
        "Primitive Assembly: Form triangles",
        "Rasterization: Convert to fragments",
        "Fragment Processing: Apply color & lighting",
    };
    for (std::size_t i = 0; i < stages.size(); ++i)
        std::cout << std::format("{}. {}\n", i + 1, stages[i]);
}

void run_demo(int choice) {
    const auto it = std::ranges::find_if(kTechniques,
        [choice](const TechniqueInfo& t) { return static_cast<int>(t.tech) == choice; });

    if (it == kTechniques.end()) {
        std::cout << "Invalid choice\n";
        return;
    }

    std::cout << std::format("\n[{}]\n", it->header);
    for (const auto& line : it->lines) std::cout << line << '\n';
}

[[nodiscard]] std::optional<std::string_view> technique_name(int choice) noexcept {
    const auto it = std::ranges::find_if(kTechniques,
        [choice](const TechniqueInfo& t) { return static_cast<int>(t.tech) == choice; });
    if (it == kTechniques.end()) return std::nullopt;
    return it->name;
}

[[nodiscard]] int get_valid_choice() {
    int choice{};
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > static_cast<int>(kTechniques.size())) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << std::format("Invalid input. Enter (1-{}): ", kTechniques.size());
        } else {
            return choice;
        }
    }
}

[[nodiscard]] bool ask_repeat() {
    char ans{};
    std::cout << "\nExplore another technique? (y/n): ";
    std::cin >> ans;
    return ans == 'y' || ans == 'Y';
}

void compare_modes() {
    std::cout << "\nQuick Comparison:\n";
    std::cout << "- Rasterization: Very fast, less realistic\n";
    std::cout << "- Ray Tracing: Slower, highly realistic\n";
}

void print_header(std::string_view title) {
    std::cout << std::format("\n================================\n{}\n================================\n",
                             title);
}

void show_gpu_stats() {
    std::cout << std::format("\n[GPU Stats Simulation]\nVRAM Usage: {:.1f} GB\nGPU Load: 78%\n", 2.5);
}

void show_history(std::span<const std::string> history) {
    std::cout << "\nExplored Techniques:\n";

    if (history.empty()) {
        std::cout << "No techniques explored\n";
        return;
    }

    for (std::size_t i = 0; i < history.size(); ++i)
        std::cout << std::format("{}. {}\n", i + 1, history[i]);
}

int main() {
    std::cout << "Rendering techniques area\n";

    std::cout << "\nBasic 3D Graphics Pipeline:\n";
    std::cout << "Vertex Processing -> Primitive Assembly -> Rasterization -> Fragment Processing\n";

    show_rendering_topics();
    pipeline_breakdown();

    int explored_count = 0;
    std::vector<std::string> history;

    do {
        print_header("Rendering Technique Explorer");

        std::cout << std::format("\nSelect a technique to explore (1-{}): ", kTechniques.size());
        const int choice = get_valid_choice();

        run_demo(choice);

        if (const auto name = technique_name(choice))
            history.emplace_back(*name);

        ++explored_count;

        show_gpu_stats();

    } while (ask_repeat());

    compare_modes();

    std::cout << std::format("\nTotal techniques explored: {}\n", explored_count);
    show_history(history);

    std::cout << "\nTip: Rasterization is fast, Ray Tracing is realistic.\n";
    std::cout << "Modern game engines often combine multiple rendering techniques.\n";

    return 0;
}
