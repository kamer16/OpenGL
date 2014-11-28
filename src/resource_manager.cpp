#include "resource_manager.hpp"

# include <GL/glew.h>
# include <glm/glm.hpp>
# include <utility>

GLuint
resource_manager::load_texture(std::string&& file, unsigned texture_unit)
{
    return texture_loader_.load_texture(std::forward<std::string>(file),
                                        texture_unit);
}

void
resource_manager::enable_normals(size_t stride)
{
    // Sets shaders attribute for color (r, g, b)
    glEnableVertexAttribArray(1); // Matches layout (location = 1)
    GLvoid* offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

void
resource_manager::enable_vertices(size_t stride)
{
    // Sets shaders attribute for vertices positions
    glEnableVertexAttribArray(0); // Matches layout (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
}

void
resource_manager::load_index_buffer(vertices_idx& indices,
                                    GLuint* index_buffer_id)
{
    glGenBuffers(1, index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *index_buffer_id);
    long unsigned byte_size = sizeof (vertices_idx::value_type) * indices.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, indices.data(),
                 GL_STATIC_DRAW);
}

void
resource_manager::load_indexed_object(object& obj)
{
    // Currently materials store vao_id, as I need a different index buffer for
    // each material hence, a specific vao to keep that state.
    for (auto mat : obj.get_materials()) {
        // Using double dispatch to call the right load_indexed_data
        mat->accept(this);
    }
}
