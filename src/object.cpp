#include "object.hpp"

#include <SOIL/SOIL.h>

object::object()
{
}

void
object::add_material(material* mat)
{
    materials_.push_back(mat);
}

void
object::bind_indexed_vao(GLuint program_id)
{
    // Currently materials store vao_id, as I need a different index buffer for
    // each material hence, a specific vao to keep that state.
    for (auto mat : materials_) {
        mat->bind_indexed_vao(program_id);
    }
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
