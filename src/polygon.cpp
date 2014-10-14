#include "polygon.hpp"
#include "utility.hpp"

#include <GL/glu.h>


polygon::polygon()
{
  glGenVertexArrays(1, &vao_id_);
  glGenBuffers(1, &index_buffer_id_);
  glGenBuffers(1, &vert_buffer_id_);
}

void polygon::draw()
{
  glBindVertexArray(vao_id_);
  glDrawElements(GL_LINES, nb_elt_, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}
