//
// Created by trist on 25.04.2018.
//

#ifndef OPENGL_TUTORIAL_SHADER_H
#define OPENGL_TUTORIAL_SHADER_H


#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>

class Shader{
public:
    unsigned int ID;
    Shader() {}
    void init(const std::string& vertexPath, const std::string& fragmentPath);
    void use();
    void setBool(const std::string &name,  bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name,  float value) const;

private:
    static std::string loadShader(const std::string& path);
    static void createShader(const std::string& shaderCode, unsigned int &shader, GLenum type);
    static void createShaderProgram(const std::vector<unsigned int>& shaders, unsigned int& shaderProgram);
};


#endif //OPENGL_TUTORIAL_SHADER_H
