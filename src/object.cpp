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
object::draw(program& program)
{
    for (auto mat : materials_) {
        program.bind_material(*mat);
        mat->draw();
    }
}

object::~object()
{
}
