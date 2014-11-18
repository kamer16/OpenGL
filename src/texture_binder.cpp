#include "texture_binder.hpp"

// Only bind texture if it wasn't already previously bound
void
texture_binder::bind_material(material& mat)
{
    bind_texture(GL_TEXTURE0, mat.get_ambient_map_id(), cached_id_.ambient);
    bind_texture(GL_TEXTURE1, mat.get_diffuse_map_id(), cached_id_.diffuse);
    bind_texture(GL_TEXTURE2, mat.get_specular_map_id(), cached_id_.specular);
    bind_texture(GL_TEXTURE3, mat.get_dissolve_map_id(), cached_id_.dissolve);
    bind_texture(GL_TEXTURE4, mat.get_bump_map_id(), cached_id_.bump);
}

void
texture_binder::set_shader_uniforms(GLuint program_id)
{
    GLint loc0 = glGetUniformLocation(program_id, "map_Ka");
    glUniform1i(loc0, 0);

    GLint loc1 = glGetUniformLocation(program_id, "map_Kd");
    glUniform1i(loc1, 1);

    GLint loc2 = glGetUniformLocation(program_id, "map_Ks");
    glUniform1i(loc2, 2);

    GLint loc3 = glGetUniformLocation(program_id, "map_d");
    glUniform1i(loc3, 3);

    GLint loc4 = glGetUniformLocation(program_id, "map_bump");
    glUniform1i(loc4, 4);

    GLint loc5 = glGetUniformLocation(program_id, "map_pos_cam");
    glUniform1i(loc5, 5);

    GLint loc6 = glGetUniformLocation(program_id, "map_normal_cam");
    glUniform1i(loc6, 6);

    GLint loc7 = glGetUniformLocation(program_id, "map_diffuse");
    glUniform1i(loc7, 7);

    GLint loc8 = glGetUniformLocation(program_id, "map_specular");
    glUniform1i(loc8, 8);
}

void
texture_binder::bind_texture(GLenum texture_unit, GLuint new_tex,
                             GLuint& cached_tex)
{
    if (new_tex != cached_tex) {
        glActiveTexture(texture_unit);
        glBindTexture(GL_TEXTURE_2D, new_tex);
        cached_tex = new_tex;
    }
}
