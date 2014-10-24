#include "object.hpp"

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
