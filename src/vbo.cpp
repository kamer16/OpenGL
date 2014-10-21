#include <GL/glew.h>
#include <array>
#include <cassert>

#include "vbo.hpp"

template <typename T>
static void
load_data(GLuint program_id, std::vector<T> &data, const char *name)
{
    GLuint buffer_id;
    glGenBuffers(1, &buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
    long unsigned byte_size = sizeof (T) * data.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, data.data(), GL_STATIC_DRAW);
    GLint location_idx = glGetAttribLocation(program_id, name);
    glEnableVertexAttribArray(location_idx); // Matches layout (location = 1)
    size_t nb_elt = sizeof (T) / sizeof (typename T::value_type);
    glVertexAttribPointer(location_idx, nb_elt, GL_FLOAT, GL_FALSE, 0, 0);
    // TODO leaking buffer
    (void) buffer_id;
}

void bind_object(GLuint program_id, GLuint *vaoID,
                 std::vector<glm::vec3> &vertices,
                 std::vector<glm::vec3> &normals,
                 std::vector<glm::vec2> &text_coords)
{
    glGenVertexArrays(1, vaoID);
    glBindVertexArray(*vaoID);

    // Sets shaders attribute for vertices positions
    load_data(program_id, vertices, "in_position");

    // Sets shaders attribute for texture coordinates
    if (text_coords.size() == vertices.size())
        load_data(program_id, text_coords, "in_uv");
    else
        assert(text_coords.size() == 0);

    // Sets shaders attribute for texture coordinates
    load_data(program_id, normals, "in_norm");

    glBindVertexArray(0);
}
