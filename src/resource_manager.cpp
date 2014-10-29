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
resource_manager::load_indexed_data(container_vnt& vertices,
                                    vertices_idx& indices,
                                    element_resource& resource)
{
    load_vertex_buffer(vertices, &resource.vertex_buffer_id);
    load_index_buffer(indices, &resource.index_buffer_id);
    glGenVertexArrays(1, &resource.vao_id);
    glBindVertexArray(resource.vao_id);
    size_t stride = sizeof (utility::vertex_vnt);
    enable_vertex_and_normal(stride);

    // Sets shaders attribute for texture coordinates
    glEnableVertexAttribArray(2); // Matches layout (location = 2)
    GLvoid* offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3) * 2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, offset);
}

void
resource_manager::load_indexed_data(container_vn& vertices,
                                    vertices_idx& indices,
                                    element_resource& resource)
{
    load_vertex_buffer(vertices, &resource.vertex_buffer_id);
    load_index_buffer(indices, &resource.index_buffer_id);
    glGenVertexArrays(1, &resource.vao_id);
    glBindVertexArray(resource.vao_id);
    size_t stride = sizeof (utility::vertex_vn);
    enable_vertex_and_normal(stride);
}

void
resource_manager::enable_vertex_and_normal(size_t stride)
{
    // Sets shaders attribute for vertices positions
    glEnableVertexAttribArray(0); // Matches layout (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Sets shaders attribute for color (r, g, b)
    glEnableVertexAttribArray(1); // Matches layout (location = 1)
    GLvoid* offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, offset);
}

template <typename T>
void
resource_manager::load_vertex_buffer(std::vector<T>& vertices,
                                     GLuint* vert_buffer_id)
{
    glGenBuffers(1, vert_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *vert_buffer_id);
    long unsigned byte_size = sizeof (T) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices.data(), GL_STATIC_DRAW);
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
