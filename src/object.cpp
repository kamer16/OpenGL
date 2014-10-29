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
object::bind_indexed_vao(resource_manager& rm)
{
    // Currently materials store vao_id, as I need a different index buffer for
    // each material hence, a specific vao to keep that state.
    for (auto mat : materials_) {
        if (mat->get_vertices_vnt().size())
            rm.load_indexed_data(mat->get_vertices_vnt(), mat->get_indices(),
                                 mat->get_resource());
        else
            rm.load_indexed_data(mat->get_vertices_vn(), mat->get_indices(),
                                 mat->get_resource());
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
