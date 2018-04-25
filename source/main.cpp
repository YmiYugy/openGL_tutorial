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
#include "Texture.h"



class TriangleApplication {
public:
    void run() {
        init();
        mainLoop();
        cleanup();
    }

private:
    const std::string RES_PATH = "../res/";
    const std::string SHADER_PATH = RES_PATH + "shaders/";
    const std::string TEX_PATH = RES_PATH + "textures/";

    GLFWwindow *window;
    std::vector<float> vertices1 = {
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };


    std::vector<unsigned int> indices = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    Shader shaderProgram;
    Texture texture;
    std::vector<unsigned int> VAO;
    std::vector<unsigned int> VBO;
    std::vector<unsigned int> EBO;


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
        glDeleteVertexArrays(VAO.size(), VAO.data());
        glDeleteBuffers(VBO.size(), VBO.data());
        glDeleteBuffers(EBO.size(), EBO.data());


        glfwTerminate();
    }

    void initWindow() {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        //glfwWindowHint(GLFW_SAMPLES, 8);

        window = glfwCreateWindow(700, 600, "LearnOpenGL", nullptr, nullptr);

        if (window == nullptr) {
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

        //glEnable(GL_MULTISAMPLE);
    }

    void initGraphicsPipeline() {
        shaderProgram.init(SHADER_PATH + "shader.vert", SHADER_PATH + "shader.frag");
        VAO.resize(1);
        VBO.resize(1);
        EBO.resize(1);

        // init VBO and VAO and EBO
        glGenVertexArrays(VAO.size(), VAO.data());
        glGenBuffers(VBO.size(), VBO.data());
        glGenBuffers(EBO.size(), EBO.data());

        glBindVertexArray(VAO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices1.size(), vertices1.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);


        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texture coord attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);



        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        texture.init({TEX_PATH + "wall.jpg", TEX_PATH + "grass.jpg"});
        shaderProgram.use();
        for(int i = 0; i < texture.texture.size(); i++){
            shaderProgram.setInt("texture"+(i+1), 1);
        }

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

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.texture[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture.texture[1]);
        shaderProgram.use();
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);




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