#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

const char* vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 pos;
    layout(location = 1) in vec3 col;

    out vec3 vertColor;

    void main() {
        gl_Position = vec4(pos, 1.0);
        vertColor = col;
    }
)";
const char* fragmentShaderSrc = R"(
    #version 330 core
    in vec3 vertColor;
    out vec4 fragColor;

    uniform float uTime;

    void main() {
        // culoarea pulseaza in timp cu sin(uTime)
        float pulse = 0.5 + 0.5 * sin(uTime * 2.0);
        fragColor = vec4(vertColor * pulse, 1.0);
    }
)";
unsigned int compileShader(unsigned int type, const char* src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int ok;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetShaderInfoLog(id, 512, nullptr, log);
        std::cerr << "Shader error:\n" << log << "\n";
    }
    return id;
}
unsigned int createProgram(const char* vs, const char* fs) {
    unsigned int v = compileShader(GL_VERTEX_SHADER, vs);
    unsigned int f = compileShader(GL_FRAGMENT_SHADER, fs);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog, v);
    glAttachShader(prog, f);
    glLinkProgram(prog);

    int ok;
    glGetProgramiv(prog, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        glGetProgramInfoLog(prog, 512, nullptr, log);
        std::cerr << "Link error:\n" << log << "\n";
    }

    // dupa link nu mai ai nevoie de shader objects
    glDeleteShader(v);
    glDeleteShader(f);

    return prog;
}
void framebufferSizeCallback(GLFWwindow* w, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* w) {
    if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(w, true);
}
int main() {
    // --- init GLFW ---
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Window creation failed\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "GLAD init failed\n";
        return -1;
    }
    float vertices[] = {
        // pozitie          // culoare
        -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // stanga jos  — rosu
         0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // dreapta jos — verde
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // sus         — albastru
    };
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // atribut 0: pozitie (3 floats)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // atribut 1: culoare (3 floats, offset dupa primii 3)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0); // unbind

    unsigned int shader = createProgram(vertexShaderSrc, fragmentShaderSrc);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        float t = (float)glfwGetTime();
        glUseProgram(shader);
        glUniform1f(glGetUniformLocation(shader, "uTime"), t);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);
    glfwTerminate();

    return 0;
}
