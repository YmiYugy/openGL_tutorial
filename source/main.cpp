//
// Created by trist on 23.04.2018.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>
#include <fstream>
#include <thread>
#include <vector>
#include <sstream>

#include "shader.h"


class TriangleApplication {
public:
    void run() {
        init();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow *window;
    std::vector<float> vertices1 = {
            // positions         // colors
            0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
            0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
    };

    /*
    std::vector<unsigned int> indices = {
            0, 3, 2,   // first triangle
            1, 4, 3    // second triangle
    };
     */
    Shader shaderProgram;
    std::vector<unsigned int> VAO;
    std::vector<unsigned int> VBO;
    //std::array<unsigned int, 1> EBO;


    void init() {
        initWindow();
        initGL();
        initGraphicsPipeline();
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
        glDeleteVertexArrays(2, VAO.data());
        glDeleteBuffers(2, VBO.data());
        //glDeleteBuffers(1, &EBO);


        glfwTerminate();
    }

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_SAMPLES, 8);

        window = glfwCreateWindow(700, 600, "LearnOpenGL", nullptr, nullptr);

        if (window == NULL) {
            glfwTerminate();
            throw std::runtime_error("failed to create GLFW window\n");
        }
        glfwSetWindowUserPointer(window, this);
    }

    void initGL() {
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("failed to initialize GLAD\n");
        }


        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowRefreshCallback(window, window_refresh_callback);

        glEnable(GL_MULTISAMPLE);
    }

    void initGraphicsPipeline() {
        shaderProgram.init("../shaders/shader.vert", "../shaders/shader.frag");
        VAO.resize(1);
        VBO.resize(1);

        // init VBO and VAO and EBO
        glGenVertexArrays(VAO.size(), VAO.data());
        glGenBuffers(VBO.size(), VBO.data());
        //glGenBuffers(EBO.size(), EBO.data());

        glBindVertexArray(VAO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices1.size(), vertices1.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3* sizeof(float)));
        glEnableVertexAttribArray(1);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);


        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    }




    static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);
    }

    static void window_refresh_callback(GLFWwindow *window) {
        TriangleApplication *app = reinterpret_cast<TriangleApplication *>(glfwGetWindowUserPointer(window));
        if (app) {
            app->draw();
            glfwSwapBuffers(window);
        }

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

        glBindVertexArray(VAO[0]);
        shaderProgram.use();
        shaderProgram.setFloat("offset", 0.0f);
        glDrawArrays(GL_TRIANGLES, 0, 3);



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