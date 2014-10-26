#include "object.hpp"

#include <SOIL/SOIL.h>

object::object()
{
}

auto object::get_vertices_vtn() -> container_vtn&
{
    return vertices_vtn_;
}

auto object::get_vertices_vn() -> container_vn&
{
    return vertices_vn_;
}

auto object::get_indices() -> vertices_idx&
{
    return indices_;
}

template <typename T>
void
object::load_index_buffer(std::vector<T>& indices)
{
    glGenBuffers(1, &index_buffer_id_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);
    long unsigned byte_size = sizeof (T) * indices.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, indices.data(), GL_STATIC_DRAW);
}

template <typename T>
void
object::load_vertex_buffer(GLuint program_id, std::vector<T>& vertices,
                           bool has_text_coord)
{
    glGenBuffers(1, &vert_buffer_id_);
    GLint pos_location = glGetAttribLocation(program_id, "in_position");
    glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id_);
    long unsigned byte_size = sizeof (T) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices.data(), GL_STATIC_DRAW);

    // Sets shaders attribute for vertices positions
    glEnableVertexAttribArray(pos_location); // Matches layout (location = 0)
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof (T), 0);

    // Sets shaders attribute for texture coordinates
    if (has_text_coord)
    {
        GLint uv_idx = glGetAttribLocation(program_id, "in_uv");
        if (uv_idx != -1) {
            glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
            GLvoid *offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
            glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, sizeof (T),
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
        glVertexAttribPointer(norm_idx, 3, GL_FLOAT, GL_FALSE, sizeof (T), offset);
    }
}

void
object::bind_indexed_vao(GLuint program_id)
{
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);
    if (vertices_vtn_.size())
        load_vertex_buffer(program_id, vertices_vtn_, 1);
    else
        load_vertex_buffer(program_id, vertices_vn_, 0);
    load_index_buffer(indices_);
    glBindVertexArray(0);
}

const glm::mat4&
object::get_model_mat()
{
    return model_mat_;
}

void
object::set_model_mat(glm::mat4& model_mat)
{
    model_mat_ = model_mat;
}


void
object::translate(const glm::vec3& dir)
{
    model_mat_ = glm::translate(model_mat_, dir);
}

void
object::rotate(float degrees, const glm::vec3& dir)
{
    model_mat_ = glm::rotate(model_mat_, glm::radians(degrees), dir);
}

void
object::scale(const glm::vec3& vec)
{
    model_mat_ = glm::scale(vec);
}

object::~object()
{
}
