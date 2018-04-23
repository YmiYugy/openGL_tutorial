//
// Created by trist on 23.04.2018.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>


class TriangleApplication {
public:
    void run() {
        init();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow *window;

    void init() {
        initWindow();
        initGL();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            processInput();
            draw();
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    void cleanup() {
        glfwTerminate();
    }

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(700, 600, "LearnOpenGL", nullptr, nullptr);

        if (window == NULL) {
            glfwTerminate();
            throw std::runtime_error("failed to create GLFW window\n");
        }
    }

    void initGL() {
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("failed to initialize GLAD\n");
        }

        glViewport(0, 0, 800, 600);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    }

    static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    void processInput() {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    void draw() {
        // clear window
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }


};

int main() {
    TriangleApplication app;
    try {
        app.run();
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}