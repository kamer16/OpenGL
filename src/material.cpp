#include "material.hpp"
#include "utility.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include <string>
#include <fstream>
#include <iostream>

void
material::draw()
{
    glBindVertexArray(resource.vao_id);
    int nb_elt = static_cast<int>(indices.size());
    glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

auto material::get_vertices_vnt() -> container_vnt&
{
    return vertices_vnt;
}

auto material::get_resource() -> element_resource&
{
    return resource;
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

GLuint& material::get_bump_id()
{
    return bump_id;
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
