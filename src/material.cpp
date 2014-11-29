#include "material.hpp"
#include "utility.hpp"
#include "resource_manager.hpp"

#include <glm/glm.hpp>

#include <string>
#include <fstream>
#include <iostream>

material_vnta::~material_vnta()
{
}
auto material_vnta::get_vertices() -> container_vnta&
{
    return vertices_vnta;
}
void material_vnta::accept(resource_manager* rm)
{
    rm->load_indexed_data(*this);
}

material_vnt::~material_vnt()
{
}
auto material_vnt::get_vertices() -> container_vnt&
{
    return vertices_vnt;
}
void material_vnt::accept(resource_manager* rm)
{
    rm->load_indexed_data(*this);
}

material_vn::~material_vn()
{
}
auto material_vn::get_vertices() -> container_vn&
{
    return vertices_vn;
}
void material_vn::accept(resource_manager* rm)
{
    rm->load_indexed_data(*this);
}

material_v::~material_v()
{
}
auto material_v::get_vertices() -> container_v&
{
    return vertices_v;
}
void material_v::accept(resource_manager* rm)
{
    rm->load_indexed_data(*this);
}


////////////////////////////////////
// MATERIAL BASE CLASS DEFINITION //
////////////////////////////////////

material::~material()
{
}

material::material(render_type type)
    : render_type_(type)
{
}

void
material::accept(resource_manager* rm)
{
    (void) rm;
    // Ideally this class should be virtual void, however obj_loading uses a hack
    // that needs to first instanciate a metrial class, as at the time of
    // creation, obj_loader does not know what type of material to create.
    std::cerr << "Material : material::accept should never be called as"
        " this class should never be instanciated!!\n";
    exit (1);
}

auto material::get_resource() -> element_resource&
{
    return resource;
}

void
material::draw(GLenum mode)
{
    glBindVertexArray(resource.vao_id);
    int nb_elt = static_cast<int>(indices.size());
    glDrawElements(mode, nb_elt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

glm::vec4&
material::get_ambient()
{
    return ambient_;
}

glm::vec4&
material::get_specular()
{
    return specular_;
}

glm::vec4&
material::get_diffuse()
{
    return diffuse_;
}

float&
material::get_shininess()
{
    return shininess;
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

GLuint& material::get_dissolve_map_id()
{
    return dissolve_map_id;
}

GLuint& material::get_bump_map_id()
{
    return bump_map_id;
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

render_type
material::get_render_type()
{
    return render_type_;
}

void
material::set_render_type(render_type type)
{
    render_type_ = type;
}
