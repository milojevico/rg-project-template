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
    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(red, green, blue, alpha);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
