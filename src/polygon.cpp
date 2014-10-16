#include "polygon.hpp"
#include "utility.hpp"

#include <GL/glu.h>


polygon::polygon(GLenum mode)
{
  glGenVertexArrays(1, &vao_id_);
  glGenBuffers(1, &index_buffer_id_);
  glGenBuffers(1, &vert_buffer_id_);
  mode_ = mode;
}

void polygon::draw()
{
  glBindVertexArray(vao_id_);
  glDrawElements(mode_, nb_elt_, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

polygon *
polygon::coordinate_new(GLuint program_id)
{
  polygon *coord = new polygon(GL_LINES);
  std::array<unsigned int, 6> cube_idxs = {{
    0, 1, 0, 2, 0, 3
  }};
  std::array<utility::color_vert, 4> cube_verts = {{
      /* Vertex coord     ,  Vertex color */
    { { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.5f } },
    { { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
    { { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
    { { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } }
  }};
  coord->load_vertex_buffer(program_id, cube_verts);
  coord->load_index_buffer(cube_idxs);
  return coord;
}

polygon *
polygon::quad_xz_new(GLuint program_id)
{
  polygon *coord = new polygon(GL_TRIANGLES);
  std::array<unsigned int, 6> cube_idxs = {{
    0, 1, 2, 2, 0, 3
  }};
  std::array<utility::color_vert, 4> cube_verts = {{
      /* Vertex coord     ,  Vertex color */
    { { -0.5f, 0.0f, -0.5f }, { 0.5f, 0.0f, 0.0f } },
    { { 0.5f, 0.0f, -0.5f }, { 0.0f, 0.0f, 0.5f } },
    { { 0.5f, 0.0f, 0.5f }, { 0.0f, 0.5f, 0.0f } },
    { { -0.5f, 0.0f, 0.5f }, { 0.5f, 0.5f, 0.5f } }
  }};
  coord->load_vertex_buffer(program_id, cube_verts);
  coord->load_index_buffer(cube_idxs);
  return coord;
}
