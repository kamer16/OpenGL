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

