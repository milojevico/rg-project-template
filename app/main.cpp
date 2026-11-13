// @formatter:off

#include <glad/glad.h>
#include <iostream>
#include <math.h>
#include <ostream>

// @formatter:on
#include <GLFW/glfw3.h>
#include <vector>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void draw_polygon(int n);

unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
float red = 0.2f;
float green = 0.3f;
float blue = 0.3f;
const float alpha = 1.0f;
unsigned int VAO2, VBO2, EBO2;
unsigned int polygonIndex = 0;
int polygonN = 0;

const char *vertex_shader_source = R"s(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
out vec4 outColor;

void main() {
    gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0);
    outColor = aColor;
}
)s";
const char *fragment_shader_source = R"s(
#version 330 core
out vec4 FragColor;
in vec4 outColor;

void main() {
    FragColor = vec4(outColor);
}
)s";


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
            -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f,
            -0.8f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f, 0.1f,
            -0.8f, -0.7f, 0.0f, 1.0f, 1.0f, 1.0f, 0.1f,
            -0.7f, -0.8f, 0.0f, 1.0f, 1.0f, 1.0f, 0.1f,
            -0.7f, -0.7f, 0.0f, 1.0f, 1.0f, 1.0f, 0.1f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.2f,
            -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.2f,
            0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.2f,
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.2f,
            -1.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.2f,
            -1.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.2f,
            0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.2f,
            0.0f, 0.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.2f,
            1.0f, 1.0f, 0.0f, 0.2f, 0.7f, 0.8f, 0.2f,
            1.0f, 0.0f, 0.0f, 0.2f, 0.7f, 0.8f, 0.2f,
            0.0f, 1.0f, 0.0f, 0.2f, 0.7f, 0.8f, 0.2f,
            0.0f, 0.0f, 0.0f, 0.2f, 0.7f, 0.8f, 0.2f
            //@ formater:on
    };

    unsigned int indices[] = {
            7, 8, 9,
            8, 9, 10,
            11, 12, 13,
            11, 13, 14,
            15, 16, 17,
            16, 17, 18,
            0, 1, 2,
            3, 4, 5,
            4, 5, 6};

    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO2);

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void *) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 27, GL_UNSIGNED_INT, 0);

        if (polygonN != 0) {
            draw_polygon(polygonN);
        }

        if (polygonN != 0) {
            glBindVertexArray(VAO2);
            glDrawElements(GL_TRIANGLES, polygonIndex,
                           GL_UNSIGNED_INT, 0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);
    glDeleteBuffers(1, &EBO);

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
    for (int i = 3; i <= 9; i++) {
        if (glfwGetKey(window, GLFW_KEY_0 + i) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
            polygonN = i;
        }
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

void draw_polygon(int n) {
    std::vector<float> vertices(7 * (n + 1));
    std::vector<unsigned int> indices(3 * n);
    for (int i = 0; i < n; i++) {
        indices[3 * i + 1] = i + 1;
        indices[3 * i + 2] = i + 2;
        indices[3 * i] = 0;
        if (i + 2 == n + 1) {
            indices[3 * i + 2] = 1;
        }
    }
    float r = 0.5f;
    float phi = 2.0 * M_PI / n;
    float x, y;
    vertices[0] = 0.0f;
    vertices[1] = 0.0f;
    vertices[2] = 0.0f;
    vertices[3] = 1.0f;
    vertices[4] = 1.0f;
    vertices[5] = 1.0f;
    vertices[6] = 1.0f;
    for (int i = 1; i <= n; i++) {
        x = r * sin(phi * (i - 1));
        y = r * cos(phi * (i - 1));
        vertices[7 * i] = x;
        vertices[7 * i + 1] = y;
        vertices[7 * i + 2] = 0.0f;
        vertices[7 * i + 3] = 1.0f;
        vertices[7 * i + 4] = 1.0f;
        vertices[7 * i + 5] = 1.0f;
        vertices[7 * i + 6] = 1.0f;
    }

    glBindVertexArray(VAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(float),
                 vertices.data(),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(),
                 GL_DYNAMIC_DRAW);

    polygonIndex = indices.size();

    glBindVertexArray(0);
}
