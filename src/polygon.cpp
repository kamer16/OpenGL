#include "polygon.hpp"
#include "utility.hpp"

#include <GL/glu.h>

polygon::polygon(GLenum mode)
{
    glGenVertexArrays(1, &resource_.vao_id);
    glGenBuffers(1, &resource_.index_buffer_id);
    glGenBuffers(1, &resource_.vertex_buffer_id);
    mode_ = mode;
}

polygon::~polygon()
{
}

void polygon::draw(program& program)
{
    (void) program;
    glBindVertexArray(resource_.vao_id);
    int nb_elt = static_cast<int>(indices_.size());
    glDrawElements(mode_, nb_elt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

polygon*
make_coordinate_polygon()
{
    polygon *coord = new polygon(GL_LINES);
    auto& indices = coord->get_indices();
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(3); indices.push_back(4); indices.push_back(5);
    auto& vertices = coord->get_vertices();
    using namespace glm;
    using namespace utility;
    vertices.push_back(vertex_vn(vec3(0.0f, 0.0f, 0.0f   ), vec3(1.0f, 0.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(800.0f, 0.0f, 0.0f ), vec3(1.0f, 0.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(0.0f, 0.0f, 0.0f   ), vec3(0.0f, 1.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(0.0f, 800.0f, 0.0f ), vec3(0.0f, 1.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(0.0f, 0.0f, 0.0f   ), vec3(0.0f, 0.0f, 1.0f)));
    vertices.push_back(vertex_vn(vec3(0.0f, 0.0f, 800.0f ), vec3(0.0f, 0.0f, 1.0f)));
    return coord;
}

polygon*
make_quad_xy_polygon()
{
    polygon *quad_xy = new polygon(GL_TRIANGLES);
    auto& indices = quad_xy->get_indices();
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(2); indices.push_back(3); indices.push_back(0);
    auto& vertices = quad_xy->get_vertices();
    using namespace glm;
    using namespace utility;
    vertices.push_back(vertex_vn(vec3(-1.0f, -1.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3( 1.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.5f)));
    vertices.push_back(vertex_vn(vec3( 1.0f,  1.0f, 0.0f), vec3(0.0f, 0.5f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(-1.0f,  1.0f, 0.0f), vec3(0.5f, 0.5f, 0.5f)));
    return quad_xy;
}

polygon*
make_quad_xz_polygon()
{
    polygon *quad_xz = new polygon(GL_TRIANGLES);
    auto& indices = quad_xz->get_indices();
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(0); indices.push_back(3); indices.push_back(2);
    auto& vertices = quad_xz->get_vertices();
    using namespace glm;
    using namespace utility;
    vertices.push_back(vertex_vn(vec3(-9.5f, 0.0f, -9.5f), vec3(0.5f, 0.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3( 9.5f, 0.0f, -9.5f), vec3(0.0f, 0.0f, 0.5f)));
    vertices.push_back(vertex_vn(vec3( 9.5f, 0.0f,  9.5f), vec3(0.0f, 0.5f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(-9.5f, 0.0f,  9.5f), vec3(0.5f, 0.5f, 0.5f)));
    return quad_xz;
}

polygon*
make_cube_polygon()
{
    polygon *cube = new polygon(GL_TRIANGLES);
    auto& indices = cube->get_indices();
    auto& vertices = cube->get_vertices();
    indices.push_back(0); indices.push_back(2); indices.push_back(1);
    indices.push_back(0); indices.push_back(3); indices.push_back(2);
    indices.push_back(4); indices.push_back(5); indices.push_back(6);
    indices.push_back(4); indices.push_back(6); indices.push_back(7);
    indices.push_back(0); indices.push_back(7); indices.push_back(3);
    indices.push_back(0); indices.push_back(4); indices.push_back(7);
    indices.push_back(1); indices.push_back(2); indices.push_back(6);
    indices.push_back(1); indices.push_back(6); indices.push_back(5);
    indices.push_back(0); indices.push_back(1); indices.push_back(5);
    indices.push_back(0); indices.push_back(5); indices.push_back(4);
    indices.push_back(3); indices.push_back(7); indices.push_back(6);
    indices.push_back(3); indices.push_back(6); indices.push_back(2);

    using namespace glm;
    vertices.push_back({ vec3( 0.5f,  0.5f,  0.5f), vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back({ vec3( 0.5f,  0.5f, -0.5f), vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back({ vec3( 0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back({ vec3( 0.5f, -0.5f,  0.5f), vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back({ vec3(-0.5f,  0.5f,  0.5f), vec3(1.0f, 0.0f, 0.0f) });
    vertices.push_back({ vec3(-0.5f,  0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back({ vec3(-0.5f, -0.5f, -0.5f), vec3(1.0f, 1.0f, 0.0f) });
    vertices.push_back({ vec3(-0.5f, -0.5f,  0.5f), vec3(1.0f, 1.0f, 0.0f) });
    return cube;
}

polygon*
make_sphere_polygon(unsigned stacks, unsigned slices, float radius)
{
    polygon *sphere = new polygon(GL_TRIANGLES);
    auto& indices = sphere->get_indices();
    auto& vertices = sphere->get_vertices();
    // Calc The Vertices
    for (unsigned i = 0; i <= stacks; ++i) {
        float V  = i / static_cast<float>(stacks);
        float phi = V * glm::pi<float>();

        // Loop Through slices
        for (unsigned j = 0; j <= slices; ++j) {

            float U = j / static_cast<float>(slices);
            float theta = U * (glm::pi<float>() * 2);

            // Calc The Vertex Positions
            float x = cosf(theta) * sinf(phi);
            float y = cosf(phi);
            float z = sinf(theta) * sinf(phi);

            // Push Back Vertex Data
            vertices.push_back ({ glm::vec3 (x, y, z) * radius,
                                glm::normalize(glm::vec3(x, y, z)) });
        }
    }

    // Calc The Index Positions
    for (unsigned i = 0; i < slices * stacks + slices; ++i) {
        indices.push_back(i);
        indices.push_back(i + slices + 1);
        indices.push_back(i + slices);

        indices.push_back(i + slices + 1);
        indices.push_back(i);
        indices.push_back(i + 1);
    }
    return sphere;
}

auto
polygon::get_vertices() -> container_vn&
{
    return vertices_;
}

auto
polygon::get_indices() -> vertices_idx&
{
    return indices_;
}

auto
polygon::get_resource() -> element_resource&
{
    return resource_;
}
