#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// ✅ ADDED
#include <string>
#include <chrono>

// ===== VERY SMALL NEW ADDITIONS =====
#include <cmath>
#include <iomanip>

// ✅ ADDED: Resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

// ---------------- NEW SMALL ADDITIONS ----------------

// process input (ESC to close + NEW: wireframe toggle)
void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // ✅ ADDED: Toggle wireframe mode
    static bool wireframe = false;
    static bool keyPressed = false;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && !keyPressed) {
        wireframe = !wireframe;
        keyPressed = true;

        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        std::cout << "Wireframe mode: "
                  << (wireframe ? "ON" : "OFF") << "\n";
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
        keyPressed = false;
}

// check shader compilation errors
void check_shader_errors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[512];

    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION (" << type << ")\n"
                      << infoLog << "\n";
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING\n"
                      << infoLog << "\n";
        }
    }
}

// ✅ ADDED: OpenGL error checker
void check_gl_error() {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL Error: " << err << "\n";
    }
}

// -----------------------------------------------------

// ===== VERY SMALL EXTRA HELPERS =====

// Print OpenGL information
void print_opengl_info() {
    std::cout << "Renderer: "
              << glGetString(GL_RENDERER) << "\n";

    std::cout << "OpenGL Version: "
              << glGetString(GL_VERSION) << "\n";
}

// Simple runtime timer
float get_runtime_seconds(
    const std::chrono::high_resolution_clock::time_point& start)
{
    auto now = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<float>(now - start).count();
}

// ====================================

// ✅ ADDED: Shader sources
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main()
{
    gl_Position = vec4(aPos, 1.0);
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main()
{
    FragColor = vec4(0.2f, 0.7f, 0.3f, 1.0f);
}
)";

int main(){

    // Initialize GLFW
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Window", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    // ✅ ADDED
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load OpenGL functions using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

    // ✅ ADDED: Set viewport
    glViewport(0, 0, 800, 600);

    // ===== VERY SMALL EXTRA ADDITION =====
    print_opengl_info();
    // =====================================

    // ✅ ADDED: Triangle data
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };

    // VAO + VBO
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ✅ ADDED: Shader compilation
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    check_shader_errors(vertexShader, "VERTEX");

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    check_shader_errors(fragmentShader, "FRAGMENT");

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    check_shader_errors(shaderProgram, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ✅ ADDED: FPS counter setup
    auto lastTime = std::chrono::high_resolution_clock::now();

    // ===== VERY SMALL EXTRA ADDITION =====
    auto appStart = std::chrono::high_resolution_clock::now();
    // =====================================

    int frames = 0;

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        process_input(window);

        // ✅ ADDED: animated background color
        float time = glfwGetTime();
        float green = (std::sin(time) / 2.0f) + 0.5f;

        glClearColor(0.1f, green, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // ✅ ADDED: FPS calculation
        frames++;
        auto currentTime = std::chrono::high_resolution_clock::now();

        float elapsed =
            std::chrono::duration<float>(currentTime - lastTime).count();

        if (elapsed >= 1.0f) {

            // ===== VERY SMALL EXTRA OUTPUT =====
            float runtime = get_runtime_seconds(appStart);
            // ===================================

            std::cout << "FPS: " << frames
                      << " | Runtime: "
                      << std::fixed << std::setprecision(1)
                      << runtime << "s\n";

            frames = 0;
            lastTime = currentTime;
        }

        // ✅ ADDED: OpenGL error check
        check_gl_error();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ---------------- CLEANUP ----------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // -----------------------------------------

    std::cout << "OpenGL resources cleaned successfully\n";

    glfwTerminate();
    return 0;
}
