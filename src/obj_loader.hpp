#ifndef OBJ_LOADER_HPP
# define OBJ_LOADER_HPP

# include <vector>
# include <iostream>
# include <sstream>
# include <fstream>
# include <unordered_set>

# include "object.hpp"
# include "material.hpp"

// A structure used for storing the indices of each vertex in the face
// declaration of a polygon.
struct s_vertex_idx
{
    // Vertex index
    size_t v;
    // Texture index
    size_t t;
    // Normal index
    size_t n;
};

class hash_ptr
{
public:
    size_t operator() (const std::tuple<size_t, size_t, size_t>& tup) const
    {
        size_t a = std::get<0>(tup);
        size_t b = std::get<1>(tup);
        size_t c = std::get<2>(tup);
        std::hash<size_t> obj;
        return ((obj(a) ^ (obj(b) << 1)) >> 1) ^ obj(c);
    }
};


class obj_loader
{
public:
    using index_map = std::unordered_map<std::tuple<size_t, size_t, size_t>,
                                         unsigned, hash_ptr>;
    using container_vtn = std::vector<utility::vertex_vtn>;
    using container_vn = std::vector<utility::vertex_vn>;
    using vertices_idx = std::vector<unsigned>;
    using materials = std::vector<material*>;
    using container3 = std::vector<glm::vec3>;
    using container2 = std::vector<glm::vec2>;
    using container_idx = std::vector<s_vertex_idx>;
    // Reads an Obj files and stores vertices, normals, and texture coords.
    // A simple call to glDrawArarys will render the object.
    // TODO should return one object that contains all of mesh file.
    object* load_obj(std::string& file);

    // Print out vector in the mesh format
    // Mesh will only contain triangles
    void print_results();

    // Dump out the contents of each vector
    void print_triangles(container3& vertices, container2& text_coord,
                         container3& normals);
private:
    void get_vertex(std::string& str, s_vertex_idx &v_idx);
    void add_indices();
    void index_object(vertices_idx& out_idx, container_vn& out_vn);
    void index_object(vertices_idx& out_idx, container_vtn& out_vtn);
    void compute_flat_shading(unsigned i,
                              glm::vec3& cross);
    void compute_smooth_shading(std::vector<float>& normals_count,
                                size_t idx1, size_t idx2, size_t idx3,
                                glm::vec3& cross,
                                unsigned i);
    void compute_normals(char flat_shading);
    void set_material_indices(object* obj, material* mat);

    container3 vertices_;
    container3 normals_;
    container2 text_coords_;
    container_idx indices_;
    std::istringstream iss_;
    std::ifstream ifs_;
    unsigned counter_ = 0;
    // Associative map_ of all indices of object to check.  If index already
    // exists it's id can be return, otherwise a new one is created
    index_map map_;
};

#endif // OBJ_LOADER_HPP
