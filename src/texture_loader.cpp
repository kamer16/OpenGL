#include "texture_loader.hpp"

#include <SOIL/SOIL.h>
#include <iostream>

GLuint
texture_loader::load_texture(std::string&& file, unsigned texture_unit)
{
    auto it = textures_.find(file);
    if (it != textures_.end())
        return it->second;
    int width;
    int height;
    int channel;
    unsigned char *image;
    GLuint texture;
    // Set current program object for modification
    glGenTextures(1, &texture);

    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    // BUG in SOIL when channel == NULL
    image = SOIL_load_image(file.c_str(), &width, &height, &channel, SOIL_LOAD_RGB);
    if (!image) {
        std::cerr << "Unable to find file : " << file << std::endl;
        return 0;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    free(image);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    textures_[file] = texture;
    return texture;
}
