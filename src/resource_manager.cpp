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

template <typename mesh>
void
resource_manager::load_indexed_data(mesh& m)
{
    auto& vertices = m.get_vertices();
    vertices_idx& indices = m.get_indices();
    element_resource& resource = m.get_resource();

    glGenVertexArrays(1, &resource.vao_id);
    glBindVertexArray(resource.vao_id);
    load_vertex_buffer(vertices, &resource.vertex_buffer_id);
    load_index_buffer(indices, &resource.index_buffer_id);
    size_t stride = sizeof (typename mesh::value_type);
    // A mesh always has vertices so we always send them to shader
    enable_vertices(stride);
    if (mesh::has_normal)
        enable_normals(stride);

    if (mesh::has_texture) {
        // Sets shaders attribute for texture coordinates
        glEnableVertexAttribArray(2); // Matches layout (location = 2)
        GLvoid* offset = reinterpret_cast<GLvoid *>(sizeof (glm::vec3) * 2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, offset);
        if (mesh::has_adjacent) {
            glEnableVertexAttribArray(3); // Matches layout (location = 3)
            offset = reinterpret_cast<GLvoid *>(sizeof (glm::vec3) * 2 +
                                                sizeof (glm::vec2));
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, stride, offset);
        }
    }
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
        if (mat->get_bump_map_id())
            load_indexed_data(static_cast<material_vnta&>(*mat));
        else if (mat->get_ambient_map_id() || mat->get_diffuse_map_id() ||
                 mat->get_specular_map_id() || mat->get_dissolve_map_id())
            load_indexed_data(static_cast<material_vnt&>(*mat));
        else
            load_indexed_data(static_cast<material_vn&>(*mat));
    }
}

void
resource_manager::load_indexed_polygon(object& p)
{
    for (auto mat : p.get_materials())
        load_indexed_data(static_cast<material_vn&>(*mat));
}
