#ifndef RESOURCE_MANAGER_HPP
# define RESOURCE_MANAGER_HPP

# include <vector>

# include "texture_loader.hpp"
# include "utility.hpp"
# include "resource.hpp"
# include "object.hpp"
# include "polygon.hpp"

class resource_manager
{
public:
    using container_vnt = std::vector<utility::vertex_vnt>;
    using container_vn = std::vector<utility::vertex_vn>;
    using vertices_idx = std::vector<unsigned>;
    template <typename mesh>
    void load_indexed_data(mesh& m);
    void load_indexed_object(object& obj);
    GLuint load_texture(std::string&& file, unsigned texture_unit);
private:
    texture_loader texture_loader_;

    void enable_normals(size_t stride);
    void enable_vertices(size_t stride);
    void load_index_buffer(vertices_idx& indices, GLuint* index_buffer_id);

    template <typename T>
    void load_vertex_buffer(std::vector<T>& vertices, GLuint* vert_buffer_id);
};

# include "resource_manager.hxx"

#endif // RESOURCE_MANAGER_HPP
