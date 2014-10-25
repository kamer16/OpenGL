#include "object.hpp"

#include <SOIL/SOIL.h>

object::object()
    :material_(nullptr)
{
}

auto object::get_vertices() -> container3&
{
    return vertices_;
}

auto object::get_normals() -> container3&
{
    return normals_;
}

template <typename T>
void
object::load_data(GLuint program_id, std::vector<T> &data, const char *name,
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

#include <iostream>

void object::bind_material()
{
    // TODO bind one texture for all objects of same type
    static GLuint tex = 0;
    if (tex == material_->diffuse_map_id)
        ;
    else {
        glBindTexture(GL_TEXTURE_2D, material_->diffuse_map_id);
        tex = material_->diffuse_map_id;
    }


}

void
object::bind_vao(GLuint program_id)
{
    glGenVertexArrays(1, &vao_id_);
    glBindVertexArray(vao_id_);

    // Sets shaders attribute for vertices positions
    load_data(program_id, vertices_, "in_position", &vert_buffer_id_);

    // Sets shaders attribute for texture coordinates
    if (text_coords_.size() == vertices_.size())
        load_data(program_id, text_coords_, "in_uv", &text_buffer_id_);
    else
        assert(text_coords_.size() == 0);

    // Sets shaders attribute for texture coordinates
    load_data(program_id, normals_, "in_norm", &norm_buffer_id_);

    glBindVertexArray(0);
}

auto object::get_text_coord() -> container2&
{
    return text_coords_;
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

void
object::set_material(std::shared_ptr<const material> material)
{
    material_ = material;
}
