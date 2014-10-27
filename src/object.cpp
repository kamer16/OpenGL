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

void
object::add_material(material* mat)
{
    materials_.push_back(mat);
}

template <typename T>
void
object::load_index_buffer(std::vector<T>& indices, GLuint* index_buffer_id)
{
    glGenBuffers(1, index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_id);
    long unsigned byte_size = sizeof (T) * indices.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, indices.data(), GL_STATIC_DRAW);
}

template <typename T>
void
object::load_vertex_buffer(std::vector<T>& vertices, GLuint* vert_buffer_id)
{
    glGenBuffers(1, vert_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *vert_buffer_id);
    long unsigned byte_size = sizeof (T) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices.data(), GL_STATIC_DRAW);

}

void
object::set_vao_attribs(GLuint program_id, bool has_text_coord, size_t stride)
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
object::bind_indexed_vao(GLuint program_id)
{
    size_t stride;
    if (vertices_vtn_.size()) {
        load_vertex_buffer(vertices_vtn_, &vertex_buffer_id_);
        stride = sizeof (utility::vertex_vtn);
    }
    else {
        load_vertex_buffer(vertices_vn_, &vertex_buffer_id_);
        stride = sizeof (utility::vertex_vn);
    }
    // Currently materials store vao_id, as I need a different index buffer for
    // each material hence, a specific vao to keep that state.
    for (auto mat : materials_) {
        glGenVertexArrays(1, &mat->vao_id);
        glBindVertexArray(mat->vao_id);
        set_vao_attribs(program_id, 1, stride);
        //glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id_);
        load_index_buffer(mat->indices, &mat->index_buffer_id);
    }
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


auto
object::get_materials() -> materials_t&
{
    return materials_;
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

void
object::draw(GLuint program_id)
{
    // TODO ABORT, materials caches all geomtries data and hence draws us
    for (auto mat : materials_) {
        mat->bind(program_id);
        glBindVertexArray(mat->vao_id);
        int nb_elt = static_cast<int>(mat->indices.size());
        glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
    }
    glBindVertexArray(0);
}

object::~object()
{
}
