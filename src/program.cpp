#include "program.hpp"

#include <glm/gtc/type_ptr.hpp>

program::program()
{
}

void
program::load_material(material& mat)
{
    texture_binder_.bind_material(mat);
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    using namespace glm;
    glUniform4fv(material_location_.specular, 1, value_ptr(mat.get_specular()));
    glUniform4fv(material_location_.diffuse, 1, value_ptr(mat.get_diffuse()));
    glUniform4fv(material_location_.ambient, 1, value_ptr(mat.get_ambient()));
    glUniform1f(material_location_.shininess, mat.get_shininess());
}
