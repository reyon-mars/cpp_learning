#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <array>
#include <span>
#include <optional>
#include <format>
#include <ranges>

enum class ShaderType {
    Vertex = 1,
    Fragment,
    Geometry,
    Compute,
};

enum class Difficulty { Beginner, Intermediate, Advanced, Unknown };

[[nodiscard]] constexpr std::string_view to_string(Difficulty d) noexcept {
    switch (d) {
        case Difficulty::Beginner:     return "Beginner";
        case Difficulty::Intermediate: return "Intermediate";
        case Difficulty::Advanced:     return "Advanced";
        case Difficulty::Unknown:      return "Unknown";
    }
    return "Unknown";
}

struct ShaderInfo {
    ShaderType type;
    std::string_view name;
    std::string_view explanation;
    Difficulty difficulty;
};

inline constexpr std::array<ShaderInfo, 4> kShaders{{
    {ShaderType::Vertex,   "Vertex Shader",   "Processes vertex positions",        Difficulty::Beginner},
    {ShaderType::Fragment, "Fragment Shader", "Determines pixel color",            Difficulty::Beginner},
    {ShaderType::Geometry, "Geometry Shader", "Modifies primitives",               Difficulty::Intermediate},
    {ShaderType::Compute,  "Compute Shader",  "General GPU computation",           Difficulty::Advanced},
}};

[[nodiscard]] std::optional<ShaderInfo> find_shader(int choice) noexcept {
    const auto it = std::ranges::find_if(kShaders,
        [choice](const ShaderInfo& s) { return static_cast<int>(s.type) == choice; });
    if (it == kShaders.end()) return std::nullopt;
    return *it;
}

void show_example_shaders() {
    constexpr std::string_view vertex_shader = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

    constexpr std::string_view fragment_shader = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

    std::cout << std::format("\nExample Vertex Shader:\n{}\n", vertex_shader);
    std::cout << std::format("\nExample Fragment Shader:\n{}\n", fragment_shader);
}

void uniform_shader_example() {
    constexpr std::string_view shader = R"(
#version 330 core
out vec4 FragColor;
uniform float time;

void main()
{
    float green = (sin(time) + 1.0) / 2.0;
    FragColor = vec4(0.0, green, 0.0, 1.0);
}
)";

    std::cout << std::format("\n[Uniform Shader Example]\n{}\n", shader);
}

void lighting_shader_example() {
    constexpr std::string_view shader = R"(
#version 330 core
out vec4 FragColor;

void main()
{
    float intensity = 0.8;
    FragColor = vec4(intensity, intensity, intensity, 1.0);
}
)";

    std::cout << std::format("\n[Basic Lighting Shader]\n{}\n", shader);
}

void explain_shader(int choice) {
    if (const auto info = find_shader(choice))
        std::cout << std::format("\n{}: {}\n", info->name, info->explanation);
    else
        std::cout << "Invalid choice\n";
}

void shader_difficulty(int choice) {
    const auto info = find_shader(choice);
    const auto difficulty = info ? info->difficulty : Difficulty::Unknown;
    std::cout << std::format("Difficulty: {}\n", to_string(difficulty));
}

[[nodiscard]] int get_valid_choice() {
    int choice{};
    while (true) {
        std::cin >> choice;
        if (std::cin.fail() || choice < 1 || choice > static_cast<int>(kShaders.size())) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << std::format("Invalid input. Enter (1-{}): ", kShaders.size());
        } else {
            return choice;
        }
    }
}

[[nodiscard]] bool ask_repeat() {
    char ans{};
    std::cout << "\nExplore another shader? (y/n): ";
    std::cin >> ans;
    return ans == 'y' || ans == 'Y';
}

void pipeline_overview() {
    std::cout << "\nGPU Pipeline Stages:\n";
    constexpr std::array stages{
        "Vertex Shader",
        "Geometry Shader (optional)",
        "Rasterization",
        "Fragment Shader",
    };
    for (const auto& stage : stages) std::cout << std::format("- {}\n", stage);
}

void print_header(std::string_view title) {
    std::cout << std::format("\n=================================\n{}\n=================================\n",
                             title);
}

void show_history(std::span<const std::string> history) {
    std::cout << "\nExplored Shader Types:\n";

    if (history.empty()) {
        std::cout << "None\n";
        return;
    }

    for (std::size_t i = 0; i < history.size(); ++i)
        std::cout << std::format("{:2}. {}\n", i + 1, history[i]);
}

int main() {
    std::cout << "Shader programming area\n";

    std::cout << "\nShader Types:\n";
    for (const auto& s : kShaders)
        std::cout << std::format("{}. {}\n", static_cast<int>(s.type), s.name);

    show_example_shaders();
    pipeline_overview();

    int explored_count = 0;
    std::vector<std::string> history;

    do {
        print_header("Shader Learning Menu");

        std::cout << std::format("\nSelect shader type to learn more (1-{}): ", kShaders.size());
        const int choice = get_valid_choice();

        explain_shader(choice);
        shader_difficulty(choice);

        if (const auto info = find_shader(choice))
            history.emplace_back(info->name);

        ++explored_count;

    } while (ask_repeat());

    uniform_shader_example();
    lighting_shader_example();

    std::cout << std::format("\nTotal shader topics explored: {}\n", explored_count);
    show_history(history);

    std::cout << "\nTip: Shaders run on GPU and control rendering pipeline stages.\n";
    std::cout << "Modern graphics engines rely heavily on optimized GLSL shaders.\n";

    return 0;
}
