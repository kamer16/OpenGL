#ifndef OBJ_LOADER_HPP
# define OBJ_LOADER_HPP

# include <vector>

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

// Reads an Obj files and stores vertices, normals, and texture coords.
// A simple call to glDrawArarys will render the object.
void load_obj(const char *file,
              std::vector<glm::vec3> &out_v,
              std::vector<glm::vec3> &out_n,
              std::vector<glm::vec2> &out_t);

// Print out vector in the mesh format
// Mesh will only contain triangles
void print_results(std::vector<glm::vec3> &vertices,
                   std::vector<glm::vec2> &text_coords,
                   std::vector<glm::vec3> &normals,
                   std::vector<s_vertex_idx> &indices);

// Dump out the contents of each vector
void
print_triangles(std::vector<glm::vec3> &vertices,
                 std::vector<glm::vec3> &normals,
                 std::vector<glm::vec2> &text_coords);

#endif // OBJ_LOADER_HPP
