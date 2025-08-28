#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shapes/TriangleShape.h"

const char* vertexShaderSrc = R"(
    #version 330 core
    layout(location = 0) in vec3 aPos;
    uniform mat4 uTransform;
    void main() {
        gl_Position = uTransform * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSrc = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(0.2, 0.7, 0.9, 1.0);
    }
)";

GLuint createShaderProgram(const char* vtxSrc, const char* fragSrc) {
    auto compile = [](GLenum type, const char* src) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(shader, 512, nullptr, log);
            std::cerr << "Shader compile error:\n" << log << std::endl;
        }
        return shader;
    };

    GLuint vs = compile(GL_VERTEX_SHADER, vtxSrc);
    GLuint fs = compile(GL_FRAGMENT_SHADER, fragSrc);

    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Program link error:\n" << log << std::endl;
    }

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL + GLFW", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to init GLAD" << std::endl;
        return -1;
    }

    TriangleShape triangle;
    triangle.setPosition({0.0f, 0.0f, 0.0f});
    triangle.setRotation(0.0f);
    triangle.setScale({2.0f, 1.0f, 1.0f});
    triangle.setColor({1.0f, 0.0f, 0.0f, 1.0f});

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 triangle.getVertices().size() * sizeof(glm::vec3),
                 triangle.getVertices().data(),
                 GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    GLuint shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "uTransform");

    float time = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        time += 0.02f;
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(1.0f + 0.5f * std::sin(time), 1.0f, 1.0f));
        transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(std::sin(time) * 0.5f, std::sin(time) * 0.5f, 0));

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(triangle.getVertices().size()));
        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
