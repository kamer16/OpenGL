#include "texture_manager.hpp"

#include <SOIL/SOIL.h>

#include <iostream>
namespace texture_manager
{

GLuint load_texture(std::string&& file, GLenum texture_unit)
{
    int width;
    int height;
    int channel;
    unsigned char *image;
    GLuint texture;
    // Set current program object for modification
    glGenTextures(1, &texture);

    glActiveTexture(texture_unit);
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
    return texture;
}

void set_shader_uniforms(GLuint program_id)
{
    glUseProgram(program_id);

    GLint loc0 = glGetUniformLocation(program_id, "texture0");
    glUniform1i(loc0, 0);
    GLint loc1 = glGetUniformLocation(program_id, "texture1");
    glUniform1i(loc1, 1);

    glUseProgram(0);
}

}
