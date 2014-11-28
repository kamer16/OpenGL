#include "polygon.hpp"
#include "utility.hpp"

#include <GL/glu.h>

namespace polygon
{

object*
make_coordinate()
{
    using namespace glm;
    using namespace utility;
    object *coord = new object(GL_LINES);

    {
    material_v* mat = new material_v();
    auto& vertices = mat->get_vertices();
    auto& indices = mat->get_indices();
    indices.push_back(0); indices.push_back(1);
    vertices.push_back(vertex_v(vec3(0.0f, 0.0f, 0.0f   )));
    vertices.push_back(vertex_v(vec3(800.0f, 0.0f, 0.0f )));
    mat->set_ambient(glm::vec4(1, 0, 0, 1));
    coord->add_material(mat);
    }

    {
    material_v* mat = new material_v();
    auto& vertices = mat->get_vertices();
    auto& indices = mat->get_indices();
    indices.push_back(0); indices.push_back(1);
    vertices.push_back(vertex_v(vec3(0.0f, 0.0f, 0.0f   )));
    vertices.push_back(vertex_v(vec3(0.0f, 800.0f, 0.0f )));
    mat->set_ambient(glm::vec4(0, 1, 0, 1));
    coord->add_material(mat);
    }

    {
    material_v*mat = new material_v();
    auto&vertices = mat->get_vertices();
    auto& indices = mat->get_indices();
    indices.push_back(0); indices.push_back(1);
    vertices.push_back(vertex_v(vec3(0.0f, 0.0f, 0.0f   )));
    vertices.push_back(vertex_v(vec3(0.0f, 0.0f, 800.0f )));
    mat->set_ambient(glm::vec4(0, 0, 1, 1));
    coord->add_material(mat);
    }

    return coord;
}

object*
make_quad_xy()
{
    material_vn* mat = new material_vn();
    auto& vertices = mat->get_vertices();
    auto& indices = mat->get_indices();

    object *quad_xy = new object(GL_TRIANGLES);
    //auto& indices = quad_xy->get_indices();
    indices.push_back(0); indices.push_back(1); indices.push_back(2);
    indices.push_back(2); indices.push_back(3); indices.push_back(0);
    //auto& vertices = quad_xy->get_vertices();
    using namespace glm;
    using namespace utility;
    vertices.push_back(vertex_vn(vec3(-1.0f, -1.0f, 0.0f), vec3(0.5f, 0.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3( 1.0f, -1.0f, 0.0f), vec3(0.0f, 0.0f, 0.5f)));
    vertices.push_back(vertex_vn(vec3( 1.0f,  1.0f, 0.0f), vec3(0.0f, 0.5f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(-1.0f,  1.0f, 0.0f), vec3(0.5f, 0.5f, 0.5f)));
    mat->set_ambient(glm::vec4(0.3, 0.3, 0.1, 1));
    quad_xy->add_material(mat);
    return quad_xy;
}

object*
make_quad_xz()
{
    material_vn* mat = new material_vn();
    auto& vertices = mat->get_vertices();
    auto& indices = mat->get_indices();

    object *quad_xz = new object(GL_TRIANGLES);
    //auto& indices = quad_xz->get_indices();
    indices.push_back(2); indices.push_back(1); indices.push_back(0);
    indices.push_back(0); indices.push_back(3); indices.push_back(2);
    //auto& vertices = quad_xz->get_vertices();
    using namespace glm;
    using namespace utility;
    vertices.push_back(vertex_vn(vec3(-9.5f, 0.0f, -9.5f), vec3(0.5f, 0.0f, 0.0f)));
    vertices.push_back(vertex_vn(vec3( 9.5f, 0.0f, -9.5f), vec3(0.0f, 0.0f, 0.5f)));
    vertices.push_back(vertex_vn(vec3( 9.5f, 0.0f,  9.5f), vec3(0.0f, 0.5f, 0.0f)));
    vertices.push_back(vertex_vn(vec3(-9.5f, 0.0f,  9.5f), vec3(0.5f, 0.5f, 0.5f)));
    mat->set_ambient(glm::vec4(0.1, 0.3, 0.3, 1));

    quad_xz->add_material(mat);
    return quad_xz;
}

object*
make_cube()
{
    object *cube = new object(GL_TRIANGLES);
    material_vn* mat = new material_vn();
    auto& vertices = mat->get_vertices();
    auto& indices = mat->get_indices();
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

    mat->set_ambient(glm::vec4(0.3, 0.1, 0.3, 1));
    cube->add_material(mat);
    return cube;
}

object*
make_sphere(unsigned stacks, unsigned slices, float radius)
{
    object *sphere = new object(GL_TRIANGLES);
    material_vn* mat = new material_vn();
    auto& vertices = mat->get_vertices();
    auto& indices = mat->get_indices();
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

    mat->set_ambient(glm::vec4(0.3, 0.8, 0.3, 1));
    sphere->add_material(mat);
    return sphere;
}

}
