#include "material.hpp"
#include "utility.hpp"
#include "texture_manager.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include <string>
#include <fstream>
#include <iostream>

GLuint
material::get_diffuse_texture()
{
    return diffuse_map_id;
}

void
material::draw(GLuint program_id)
{
    bind(program_id);
    glBindVertexArray(vao_id);
    int nb_elt = static_cast<int>(indices.size());
    glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void
material::load_index_buffer()
{
    glGenBuffers(1, &index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    long unsigned byte_size = sizeof (vertices_idx::value_type) * indices.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, indices.data(), GL_STATIC_DRAW);
}

template <typename T>
void
material::load_vertex_buffer(std::vector<T>& vertices, GLuint* vert_buffer_id)
{
    glGenBuffers(1, vert_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *vert_buffer_id);
    long unsigned byte_size = sizeof (T) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices.data(), GL_STATIC_DRAW);

}

void
material::set_vao_attribs(GLuint program_id, bool has_text_coord, size_t stride)
{
    // Sets shaders attribute for vertices positions
    GLint pos_location = glGetAttribLocation(program_id, "in_position");
    glEnableVertexAttribArray(pos_location); // Matches layout (location = 0)
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Sets shaders attribute for texture coordinates
    if (has_text_coord)
    {
        GLint uv_idx = glGetAttribLocation(program_id, "in_uv");
        if (uv_idx != -1) {
            glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
            GLvoid *offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
            glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, stride,
                                  offset);
        }
    }
    // Sets shaders attribute for color (r, g, b)
    GLint norm_idx = glGetAttribLocation(program_id, "in_norm");
    if (norm_idx != -1) {
        glEnableVertexAttribArray(norm_idx); // Matches layout (location = 1)
        GLvoid *offset;
        if (has_text_coord)
            offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3) +
                                                 sizeof (glm::vec2));
        else
            offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
        glVertexAttribPointer(norm_idx, 3, GL_FLOAT, GL_FALSE, stride, offset);
    }
}

void
material::bind_indexed_vao(GLuint program_id)
{
    size_t stride;
    if (vertices_vtn.size()) {
        load_vertex_buffer(vertices_vtn, &vertex_buffer_id);
        stride = sizeof (utility::vertex_vtn);
    }
    else {
        load_vertex_buffer(vertices_vn, &vertex_buffer_id);
        stride = sizeof (utility::vertex_vn);
    }
    // Currently materials store vao_id, as I need a different index buffer for
    // each material hence, a specific vao to keep that state.
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    set_vao_attribs(program_id, 1, stride);
    //glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id_);
    load_index_buffer();
    glBindVertexArray(0);
}

auto material::get_vertices_vtn() -> container_vtn&
{
    return vertices_vtn;
}

auto material::get_vertices_vn() -> container_vn&
{
    return vertices_vn;
}

glm::vec4&
material::get_ambient()
{
    return ambient;
}

glm::vec4&
material::get_specular()
{
    return specular;
}

glm::vec4&
material::get_diffuse()
{
    return diffuse;
}

float&
material::get_shininess()
{
    return shininess;
}

void material::bind(GLuint program_id)
{
    // TODO bind one texture for all objects of same type
    static GLuint tex = 0;
    // Only bind texture if it wasn't already previously bound
    if (tex != diffuse_map_id) {
        // TODO, texture_manager should bind textures
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, diffuse_map_id);
        tex = diffuse_map_id;
    }
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    GLint specular_idx = glGetUniformLocation(program_id, "material.specular");
    glUniform4fv(specular_idx, 1, glm::value_ptr(specular));

    GLint diffuse_idx = glGetUniformLocation(program_id, "material.diffuse");
    glUniform4fv(diffuse_idx, 1, glm::value_ptr(diffuse));

    GLint ambient_idx = glGetUniformLocation(program_id, "material.ambient");
    glUniform4fv(ambient_idx, 1, glm::value_ptr(ambient));

    GLint shininess_idx = glGetUniformLocation(program_id, "material.shininess");
    glUniform1f(shininess_idx, shininess);
}

GLuint& material::get_ambient_map_id()
{
    return ambient_map_id;
}

GLuint& material::get_diffuse_map_id()
{
    return diffuse_map_id;
}

GLuint& material::get_specular_map_id()
{
    return specular_map_id;
}

GLuint& material::get_bump_map_id()
{
    return bump_map_id;
}

GLuint& material::get_bump_id()
{
    return bump_id;
}

GLuint& material::get_dissolve_map_id()
{
    return dissolve_map_id;
}

float& material::get_dissolve()
{
    return dissolve;
}

auto material::get_idx_lut() -> index_map&
{
    return idx_lut;
}

auto material::get_indices() -> vertices_idx&
{
    return indices;
}
