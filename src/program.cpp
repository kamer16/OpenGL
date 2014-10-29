#include "program.hpp"

#include <glm/gtc/type_ptr.hpp>

program::program(texture_manager_ptr tm)
    : texture_manager_(tm)
{
}

auto program::get_texture_manager() -> const texture_manager_ptr&
{
    return texture_manager_;
}

template <typename T>
void
program::load_data(GLuint program_id, std::vector<T> &data, const char *name,
                   GLuint* buffer_id)
{
    glGenBuffers(1, buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer_id);
    long unsigned byte_size = sizeof (T) * data.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, data.data(), GL_STATIC_DRAW);
    GLint location_idx = glGetAttribLocation(program_id, name);
    glEnableVertexAttribArray(location_idx); // Matches layout (location = 1)
    size_t nb_elt = sizeof (T) / sizeof (typename T::value_type);
    glVertexAttribPointer(location_idx, nb_elt, GL_FLOAT, GL_FALSE, 0, 0);
}

void
program::load_material(material& mat)
{
    texture_manager_->bind_material(mat);
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    using namespace glm;
    glUniform4fv(material_location_.specular, 1, value_ptr(mat.get_specular()));
    glUniform4fv(material_location_.diffuse, 1, value_ptr(mat.get_diffuse()));
    glUniform4fv(material_location_.ambient, 1, value_ptr(mat.get_ambient()));
    glUniform1f(material_location_.shininess, mat.get_shininess());
}

template <typename T>
void
program::load_vertex_buffer(std::vector<T>& vertices, GLuint* vert_buffer_id)
{
    glGenBuffers(1, vert_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *vert_buffer_id);
    long unsigned byte_size = sizeof (T) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices.data(), GL_STATIC_DRAW);

}

void
program::set_vao_attribs(bool has_text_coord, size_t stride)
{
    // Sets shaders attribute for vertices positions
    GLint pos_location = glGetAttribLocation(program_id_, "in_position");
    glEnableVertexAttribArray(pos_location); // Matches layout (location = 0)
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Sets shaders attribute for texture coordinates
    if (has_text_coord)
    {
        GLint uv_idx = glGetAttribLocation(program_id_, "in_uv");
        if (uv_idx != -1) {
            glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
            GLvoid *offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
            glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, stride,
                                  offset);
        }
    }
    // Sets shaders attribute for color (r, g, b)
    GLint norm_idx = glGetAttribLocation(program_id_, "in_norm");
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

