//
// Created by trist on 25.04.2018.
//

#ifndef OPENGL_TUTORIAL_TEXTURE_H
#define OPENGL_TUTORIAL_TEXTURE_H

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <stb_image.h>
#include <string>
#include <stdexcept>

class Texture{
public:
    std::vector<unsigned int> texture;

    Texture(){}
    void init(const std::vector<std::string>& path) {
        texture.resize(path.size());

        glGenTextures(path.size(), texture.data());
        for(int i = 0; i < path.size(); i++){
            glBindTexture(GL_TEXTURE_2D, texture[i]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int width, height, nrChannels;
            unsigned char *data = stbi_load(path[i].c_str(), &width, &height, &nrChannels, 0);
            if(!data){
                throw std::runtime_error("failed to load texture " + path[i]);
            }
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            stbi_image_free(data);
        }
    }


};


#endif //OPENGL_TUTORIAL_TEXTURE_H
