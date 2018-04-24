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
            -1.0f, -1.0f, 0.0f,
             0.0f, -1.0f, 0.0f,
            -0.5f,  1.0f, 0.0f,
    };

    std::vector<float> vertices2 = {
             0.0f, -1.0f, 0.0f,
             1.0f, -1.0f, 0.0f,
             0.5f,  1.0f, 0.0f,
    };
    std::vector<unsigned int> indices = {
            0, 3, 2,   // first triangle
            1, 4, 3    // second triangle
    };
    unsigned int shaderProgram1;
    unsigned int shaderProgram2;
    std::array<unsigned int, 2> VBO;
    std::array<unsigned int, 2> VAO;
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

            glfwPollEvents();
            glfwSwapBuffers(window);
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
        // load, compile, link, and use shaders
        initShader();

        // init VBO and VAO and EBO
        glGenVertexArrays(VAO.size(), VAO.data());
        glGenBuffers(VBO.size(), VBO.data());
        //glGenBuffers(EBO.size(), EBO.data());

        glBindVertexArray(VAO[0]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices1.size(), vertices1.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices2.size(), vertices2.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof( float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    }


    void initShader() {
        auto loadShader = [](const std::string& path) -> std::string{
            std::ifstream file(path);
            if (!file.is_open()) {
                std::runtime_error("failed to open " + path);
            }
            std::string contents{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
            return contents;
        };

        auto createShader = [](const std::string &shaderCode, unsigned int &shader, GLenum type) -> void {

            const char *content = shaderCode.c_str();

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

        auto createShaderProgram = [](const std::vector<unsigned int>& shaders, unsigned int& shaderProgram) {
            shaderProgram = glCreateProgram();
            for(auto shader : shaders){
                glAttachShader(shaderProgram, shader);
            }
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
        };

        auto makeColorVec = [](float r, float g, float b, float a){
            std::stringstream ss;
            ss << "vec4(" << r << "," << g << "," << b << "," << a << ")";
            return ss.str();
        };

        auto replace = [](std::string str, const std::string& from, const std::string& to) {
            size_t start_pos = str.find(from);
            if(start_pos == std::string::npos)
                throw std::runtime_error("failed to replace " + from + " with " + to);
            str.replace(start_pos, from.length(), to);
            return str;
        };


        unsigned int vertexShader;
        unsigned int fragmentShader1;
        unsigned int fragmentShader2;
        std::string vertexShaderCode = loadShader("../shaders/shader.vert");
        std::string fragmentShaderCode = loadShader("../shaders/shader.frag");
        createShader(vertexShaderCode, vertexShader, GL_VERTEX_SHADER);
        createShader(replace(fragmentShaderCode, "#COLOR", makeColorVec(1.0f, 0.5f, 0.2f, 1.0f)), fragmentShader1, GL_FRAGMENT_SHADER);
        createShader(replace(fragmentShaderCode, "#COLOR", makeColorVec(1.0f, 1.0f, 0.0f, 1.0f)), fragmentShader2, GL_FRAGMENT_SHADER);
        createShaderProgram({vertexShader, fragmentShader1}, shaderProgram1);
        createShaderProgram({vertexShader, fragmentShader2}, shaderProgram2);


        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader1);
        glDeleteShader(fragmentShader2);
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

        glUseProgram(shaderProgram1);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO[1]);
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