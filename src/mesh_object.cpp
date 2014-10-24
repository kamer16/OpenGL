#include "mesh_object.hpp"

mesh_object::~mesh_object()
{
}

void
mesh_object::draw()
{
    glBindVertexArray(vao_id_);
    int nb_elt = static_cast<int>(vertices_.size());
    glDrawArrays(GL_TRIANGLES, 0, nb_elt);
    glBindVertexArray(0);
}
