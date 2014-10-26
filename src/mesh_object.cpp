#include "mesh_object.hpp"

mesh_object::~mesh_object()
{
}

void
mesh_object::draw()
{
    glBindVertexArray(vao_id_);
    int nb_elt = static_cast<int>(indices_.size());
    glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
