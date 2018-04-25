//
// Created by trist on 25.04.2018.
//

#include <iostream>
#include "shader.h"

void Shader::init(const std::string &vertexPath, const std::string &fragmentPath) {
    unsigned int vertexShader;
    unsigned int fragmentShader;
    createShader(loadShader(vertexPath), vertexShader, GL_VERTEX_SHADER);
    createShader(loadShader(fragmentPath), fragmentShader, GL_FRAGMENT_SHADER);

    createShaderProgram({vertexShader, fragmentShader}, ID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

std::string Shader::loadShader(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("failed to open " + path);
    }
    std::string contents{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    return contents;
}

void Shader::createShader(const std::string &shaderCode, unsigned int &shader, GLenum type) {
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
}

void Shader::createShaderProgram(const std::vector<unsigned int> &shaders, unsigned int &shaderProgram) {
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
}
