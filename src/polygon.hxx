#include "polygon.hpp"

template <typename T, std::size_t N>
void
polygon::load_index_buffer(std::array<T, N> &cube_idxs)
{
  nb_elt_ = N;
  glBindVertexArray(vao_id_);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id_);
  long unsigned byte_size = sizeof (T) * cube_idxs.size();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, cube_idxs.data(), GL_STATIC_DRAW);
  glBindVertexArray(0);
}

template <typename T, std::size_t N>
void
polygon::load_vertex_buffer(GLuint program_id, std::array<T, N> &cube_verts)
{
  glBindVertexArray(vao_id_);
  GLint pos_location = glGetAttribLocation(program_id, "in_position");
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_id_);
  long unsigned byte_size = sizeof (T) * cube_verts.size();
  glBufferData(GL_ARRAY_BUFFER, byte_size, cube_verts.data(), GL_STATIC_DRAW);

  // Sets shaders attribute for vertices positions
  glEnableVertexAttribArray(pos_location); // Matches layout (location = 0)
  glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, sizeof (T), 0);

  // Sets shaders attribute for texture coordinates
  GLint uv_idx = glGetAttribLocation(program_id, "in_uv");
  if (uv_idx != -1) {
    glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
    GLvoid *offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
    glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, sizeof (T), offset);
  }
  // Sets shaders attribute for color (r, g, b)
  GLint color_idx = glGetAttribLocation(program_id, "in_color");
  if (color_idx != -1) {
    glEnableVertexAttribArray(color_idx); // Matches layout (location = 1)
    GLvoid *offset = reinterpret_cast<GLvoid *>(sizeof (glm::vec3));
    glVertexAttribPointer(color_idx, 3, GL_FLOAT, GL_FALSE, sizeof (T), offset);
  }
  glBindVertexArray(0);
}
