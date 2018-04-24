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


class TriangleApplication {
public:
    void run() {
        init();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow *window;
    std::vector<float> vertices = {
            0.5f, 0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f, 0.5f, 0.0f   // top left
    };
    std::vector<unsigned int> indices = {
            0, 1, 3,   // first triangle
            1, 2, 3    // second triangle
    };
    unsigned int VBO;
    unsigned int shaderProgram;
    unsigned int VAO;
    unsigned int EBO;

    void init() {
        initWindow();
        initGL();
        initGraphicsPipeline();
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            processInput();
            draw();

            glfwPollEvents();
            glfwSwapBuffers(window);
        }
    }

    void cleanup() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);


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
        glfwSetWindowUserPointer(window, this);
    }

    void initGL() {
        glfwMakeContextCurrent(window);

        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("failed to initialize GLAD\n");
        }

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowRefreshCallback(window, window_refresh_callback);

    }

    void initGraphicsPipeline() {
        // load, compile, link, and use shaders
        initShader();

        // init VBO and VAO and EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    }


    void initShader() {
        auto loadShader = [](const std::string &path, unsigned int &shader, GLenum type) -> void {
            std::ifstream file(path);
            if (!file.is_open()) {
                std::runtime_error("failed to open " + path);
            }
            std::string contents{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
            const char *content = contents.c_str();

            shader = glCreateShader(type);
            glShaderSource(shader, 1, &content, nullptr);
            glCompileShader(shader);

            int success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                int length;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
                std::vector<char> log(length);
                glGetShaderInfoLog(shader, length, &length, log.data());

                throw std::runtime_error("failed to compile shader\n" + std::string(log.data()));
            }
        };
        unsigned int vertexShader;
        unsigned int fragmentShader;

        loadShader("../shaders/shader.vert", vertexShader, GL_VERTEX_SHADER);
        loadShader("../shaders/shader.frag", fragmentShader, GL_FRAGMENT_SHADER);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        int success;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (!success) {
            int length;
            glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
            std::vector<char> log(length);
            glGetProgramInfoLog(shaderProgram, length, nullptr, log.data());
            throw std::runtime_error("failed to compile shader program" + std::string(log.data()));
        }


        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

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

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
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