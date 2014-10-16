#ifndef OBJ_LOADER_HPP
# define OBJ_LOADER_HPP

# include <vector>
# include "utility.hpp"

// A structure used for storing the indices of each vertex in the face
// declaration of a polygon.
struct s_vertex_idx
{
    // Vertex index
    unsigned v;
    // Texture index
    unsigned t;
    // Normal index
    unsigned n;
};

// Reads an Obj files and stores vertices, normals, and texture coords.
// A simple call to glDrawArarys will render the object.
void load_obj(const char *file,
              std::vector<utility::vec4> &out_v,
              std::vector<utility::vec3> &out_n,
              std::vector<utility::vec3> &out_t);

// Print out vector in the mesh format
// Mesh will only contain triangles
void print_results(std::vector<utility::vec4> &vertices,
                   std::vector<utility::vec3> &normals,
                   std::vector<utility::vec3> &text_coords,
                   std::vector<s_vertex_idx> &indices);

#endif // OBJ_LOADER_HPP
