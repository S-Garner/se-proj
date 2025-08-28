#include <glad/glad.h>
#include <SFML/Window.hpp>
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
    uniform vec4 uColor;
    void main() {
        FragColor = vec4(0.2, 0.7, 0.9, 1.0);
    }
)";

GLuint createShaderProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vtxSrc, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, log);
        std::cerr << "Vertex Shader Error:\n" << log << "\n";
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, log);
        std::cerr << "Fragment Shader Error:\n" << log << "\n";
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program, 512, nullptr, log);
        std::cerr << "Shader Linking Error:\n" << log << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    // SFML context
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 3;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    if (!gladLoadGLLoader((GLADloadproc)sf::Context::getFunction)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }

    // Create the triangle shape
    TriangleShape triangle;
    triangle.setPosition({ 0.0f, 0.0f, 0.0f });
    triangle.setRotation(0.0f);
    triangle.setScale({ 2.0f, 1.0f, 1.0f });
    triangle.setColor({ 1.0f, 0.0f, 0.0f, 1.0f });

    // Set up VAO/VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Upload triangle vertices
    glBufferData(
        GL_ARRAY_BUFFER,
        triangle.getVertices().size() * sizeof(glm::vec3),
        triangle.getVertices().data(),
        GL_DYNAMIC_DRAW
    );

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(glm::vec3),
        (void*)0
    );

    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Shader program
    GLuint shaderProgram = createShaderProgram(vertexShaderSrc, fragmentShaderSrc);
    GLint transformLoc = glGetUniformLocation(shaderProgram, "uTransform");

    float time = 0.0f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        time += 0.02f;

        // Build transformation matrix using GLM
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::scale(transform, glm::vec3(1.0f + 0.5f * std::sin(time), 1.0f, 1.0f));
        transform = glm::rotate(transform, time, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::translate(transform, glm::vec3(std::sin(time) * 0.5f, std::sin(time) * 0.5f, 0));

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(
            transformLoc,
            1,
            GL_FALSE,
            glm::value_ptr(transform)
        );

        glBindVertexArray(VAO);

        glDrawArrays(
            GL_TRIANGLES, // Type we want to draw
            0,
            static_cast<GLsizei>(triangle.getVertices().size())
        );

        glBindVertexArray(0);
        window.display();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    return 0;
}
