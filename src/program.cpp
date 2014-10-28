#include "program.hpp"

#include <glm/gtc/type_ptr.hpp>

template <typename T>
void
program::load_data(GLuint program_id, std::vector<T> &data, const char *name,
                   GLuint* buffer_id)
{
    glGenBuffers(1, buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *buffer_id);
    long unsigned byte_size = sizeof (T) * data.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, data.data(), GL_STATIC_DRAW);
    GLint location_idx = glGetAttribLocation(program_id, name);
    glEnableVertexAttribArray(location_idx); // Matches layout (location = 1)
    size_t nb_elt = sizeof (T) / sizeof (typename T::value_type);
    glVertexAttribPointer(location_idx, nb_elt, GL_FLOAT, GL_FALSE, 0, 0);
}

void
program::load_material(material& mat)
{
    texture_manager_->bind_material(mat);
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    using namespace glm;
    glUniform4fv(material_location_.specular, 1, value_ptr(mat.get_specular()));
    glUniform4fv(material_location_.diffuse, 1, value_ptr(mat.get_diffuse()));
    glUniform4fv(material_location_.ambient, 1, value_ptr(mat.get_ambient()));
    glUniform1f(material_location_.shininess, mat.get_shininess());
}
