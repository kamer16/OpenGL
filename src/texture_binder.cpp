#include "texture_binder.hpp"

void
texture_binder::bind_material(material& mat)
{
    // Only bind texture if it wasn't already previously bound
    GLuint diffuse_texture = mat.get_diffuse_texture();
    if (diffuse_texture != cached_id_.diffuse) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuse_texture);
        cached_id_.diffuse = diffuse_texture;
    }
    // TODO bind other textures
}

void
texture_binder::set_shader_uniforms(GLuint program_id)
{
    glUseProgram(program_id);

    GLint loc0 = glGetUniformLocation(program_id, "texture0");
    glUniform1i(loc0, 0);
    GLint loc1 = glGetUniformLocation(program_id, "texture1");
    glUniform1i(loc1, 1);

    glUseProgram(0);
}
