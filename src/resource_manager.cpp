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

template <typename content>
void
resource_manager::load_indexed_data(std::vector<content>& vertices,
                                    vertices_idx& indices,
                                    element_resource& resource)
{
    glGenVertexArrays(1, &resource.vao_id);
    glBindVertexArray(resource.vao_id);
    load_vertex_buffer(vertices, &resource.vertex_buffer_id);
    load_index_buffer(indices, &resource.index_buffer_id);
    size_t stride = sizeof (content);
    enable_vertex_and_normal(stride);

    if (content::has_texture) {
        // Sets shaders attribute for texture coordinates
        glEnableVertexAttribArray(2); // Matches layout (location = 2)
        GLvoid* offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3) * 2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, offset);
        if (content::has_adjacent) {
        }
    }
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

void
resource_manager::load_indexed_object(object& obj)
{
    // Currently materials store vao_id, as I need a different index buffer for
    // each material hence, a specific vao to keep that state.
    for (auto mat : obj.get_materials()) {
        if (mat->get_vertices_vnt().size())
            load_indexed_data(mat->get_vertices_vnt(), mat->get_indices(),
                              mat->get_resource());
        else if (mat->get_vertices_vn().size())
            load_indexed_data(mat->get_vertices_vn(), mat->get_indices(),
                              mat->get_resource());
        else if (mat->get_vertices_vnta().size())
            load_indexed_data(mat->get_vertices_vnta(), mat->get_indices(),
                              mat->get_resource());
        else
            std::cerr << "Resource Manager: Object has no data\n";
    }
}

void
resource_manager::load_indexed_polygon(polygon& p)
{
    load_indexed_data(p.get_vertices(), p.get_indices(), p.get_resource());
}
