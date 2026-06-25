#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string_view>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <array>

using Clock = std::chrono::high_resolution_clock;

void framebuffer_size_callback(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    static bool wireframe  = false;
    static bool key_held   = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !key_held) {
        wireframe = !wireframe;
        key_held  = true;
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        std::cout << "Wireframe: " << std::boolalpha << wireframe << "\n";
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        key_held = false;
}

void check_shader_errors(GLuint shader, std::string_view type) {
    GLint success = 0;
    std::array<char, 512> log{};

    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data());
            std::cerr << "Shader compile error (" << type << "):\n" << log.data() << "\n";
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, static_cast<GLsizei>(log.size()), nullptr, log.data());
            std::cerr << "Program link error:\n" << log.data() << "\n";
        }
    }
}

void check_gl_errors() {
    for (GLenum err; (err = glGetError()) != GL_NO_ERROR;)
        std::cerr << "GL error: 0x" << std::hex << err << std::dec << "\n";
}

void print_opengl_info() {
    std::cout << "Renderer: "       << glGetString(GL_RENDERER) << "\n"
              << "OpenGL Version: " << glGetString(GL_VERSION)  << "\n";
}

[[nodiscard]] float elapsed_seconds(Clock::time_point from) {
    return std::chrono::duration<float>(Clock::now() - from).count();
}

constexpr char VERTEX_SHADER_SRC[] = R"glsl(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() { gl_Position = vec4(aPos, 1.0); }
)glsl";

constexpr char FRAGMENT_SHADER_SRC[] = R"glsl(
#version 330 core
out vec4 FragColor;
void main() { FragColor = vec4(0.2, 0.7, 0.3, 1.0); }
)glsl";

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialise GLAD\n";
        return -1;
    }

    glViewport(0, 0, 800, 600);
    print_opengl_info();

    constexpr std::array<float, 9> vertices{
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
    };

    GLuint VAO{}, VBO{};
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(vertices.size() * sizeof(float)),
                 vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &VERTEX_SHADER_SRC, nullptr);
    glCompileShader(vertShader);
    check_shader_errors(vertShader, "VERTEX");

    const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &FRAGMENT_SHADER_SRC, nullptr);
    glCompileShader(fragShader);
    check_shader_errors(fragShader, "FRAGMENT");

    const GLuint program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    check_shader_errors(program, "PROGRAM");

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    const auto app_start = Clock::now();
    auto       fps_timer  = Clock::now();
    int        frames     = 0;

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        const float t     = static_cast<float>(glfwGetTime());
        const float green = (std::sin(t) / 2.0f) + 0.5f;
        glClearColor(0.1f, green, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        ++frames;
        if (elapsed_seconds(fps_timer) >= 1.0f) {
            std::cout << "FPS=" << frames
                      << " Runtime=" << std::fixed << std::setprecision(1)
                      << elapsed_seconds(app_start) << "s\n";
            frames    = 0;
            fps_timer = Clock::now();
        }

        check_gl_errors();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);

    std::cout << "Cleaned up OpenGL resources\n";
    glfwTerminate();
    return 0;
}
