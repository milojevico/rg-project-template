// @formatter:off

#include <glad/glad.h>
#include <iostream>
#include <ostream>

// @formatter:on
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float red = 0.2f;
float green = 0.3f;
float blue = 0.3f;
const float alpha = 1.0f;


const char *vertex_shader_source = R"s(
#version 330 core
layout (location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
}
)s";
const char *fragment_shader_source = R"s(
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}
)s";

//Napraviti tako da je svaki kvadrant ekrana obojen drugom bojom
//Napisati funkciju koja crta mnogougao N na sredini ekrana
//CTRL + N - crta n-tougao na sredini ekrana gde je N broj na tastaturi

int main() {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGl", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "Failed to compile vertex shader\n"
                  << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "Failed to compile fragment shader\n"
                  << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "Failed to link program\n"
                  << infoLog << std::endl;
    }

    float vertices[] = {
            //@formater:off
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
            -0.8f, -0.8f, 0.0f,
            -0.8f, -0.7f, 0.0f,
            -0.7f, -0.8f, 0.0f,
            -0.7f, -0.7f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            -1.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 0.0f
            //@ formater:on
    };

    unsigned int indices[] = {
            0, 1, 2,
            3, 4, 5,
            4, 5, 6,
            7, 8, 9,
            8, 9, 10,
            8, 11, 12,
            8, 12, 10};

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        red = xpos / SCR_WIDTH * 1.0f;
        green = 1.0 - ypos / SCR_HEIGHT * 1.0f;
        blue = 0.0f;
    }
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        red = rand() / (float) RAND_MAX;
        green = rand() / (float) RAND_MAX;
        blue = rand() / (float) RAND_MAX;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        red += 0.01f;
        if (red > 1.00f) {
            red -= 1.00f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        green += 0.01f;
        if (green > 1.00f) {
            green -= 1.00f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        blue += 0.01f;
        if (blue > 1.00f) {
            blue -= 1.00f;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        SCR_HEIGHT += 1;
        glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
    }
    if (SCR_HEIGHT > 0 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        SCR_HEIGHT -= 1;
        glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        SCR_WIDTH += 1;
        glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
    }
    if (SCR_WIDTH > 0 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        SCR_WIDTH -= 1;
        glfwSetWindowSize(window, SCR_WIDTH, SCR_HEIGHT);
    }
}
