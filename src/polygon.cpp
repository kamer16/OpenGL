#include "polygon.hpp"
#include "utility.hpp"

#include <GL/glu.h>

polygon::polygon(GLenum mode)
{
  glGenVertexArrays(1, &vao_id_);
  glGenBuffers(1, &index_buffer_id_);
  glGenBuffers(1, &vert_buffer_id_);
  mode_ = mode;
  (void) padding_;
}

polygon::~polygon()
{
}

void polygon::draw()
{
  glBindVertexArray(vao_id_);
  glDrawElements(mode_, nb_elt_, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

object *
make_coordinate_polygon(GLuint program_id)
{
  polygon *coord = new polygon(GL_LINES);
  std::array<unsigned int, 6> coord_idxs = {{
    0, 1, 2, 3, 4, 5
  }};
  std::array<utility::color_vert, 6> coord_verts = {{
      /* Vertex coord     ,  Vertex color */
    { { 0.0f, 0.0f, 0.0f },   { 1.0f, 0.0f, 0.0f } },
    { { 800.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { 0.0f, 0.0f, 0.0f },   { 0.0f, 1.0f, 0.0f } },
    { { 0.0f, 800.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    { { 0.0f, 0.0f, 0.0f },   { 0.0f, 0.0f, 1.0f } },
    { { 0.0f, 0.0f, 800.0f }, { 0.0f, 0.0f, 1.0f } }
  }};
  coord->load_vertex_buffer(program_id, coord_verts);
  coord->load_index_buffer(coord_idxs);
  return coord;
}

object *
make_quad_xz_polygon(GLuint program_id)
{
  polygon *quad_xz = new polygon(GL_TRIANGLES);
  std::array<unsigned int, 6> quad_idxs = {{
    0, 1, 2, 2, 0, 3
  }};
  std::array<utility::color_vert, 4> quad_verts = {{
      /* Vertex quad_xz     ,  Vertex color */
    { { -9.5f, 0.0f, -9.5f }, { 0.5f, 0.0f, 0.0f } },
    { {  9.5f, 0.0f, -9.5f }, { 0.0f, 0.0f, 0.5f } },
    { {  9.5f, 0.0f,  9.5f }, { 0.0f, 0.5f, 0.0f } },
    { { -9.5f, 0.0f,  9.5f }, { 0.5f, 0.5f, 0.5f } }
  }};
  quad_xz->load_vertex_buffer(program_id, quad_verts);
  quad_xz->load_index_buffer(quad_idxs);
  return quad_xz;
}

object *
make_cube_polygon(GLuint program_id)
{
    polygon *cube = new polygon(GL_TRIANGLES);
    std::array<unsigned int, 36> cube_idxs = {{
        0, 2, 1, 0, 3, 2, // Right
        4, 5, 6, 4, 6, 7, // Left
        0, 7, 3, 0, 4, 7, // Top
        1, 6, 2, 1, 5, 6, // Bottom
        0, 5, 1, 0, 4, 5, // Front
        3, 7, 6, 3, 6, 2  // Back
    }};

    std::array<utility::color_vert, 8> cube_verts = {{
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f } },
        { { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } },
        { { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 0.0f } }
    }};

    cube->load_vertex_buffer(program_id, cube_verts);
    cube->load_index_buffer(cube_idxs);
    return cube;
}
