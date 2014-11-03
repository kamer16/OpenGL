#ifndef MATERIAL_HPP
# define MATERIAL_HPP

# include <glm/glm.hpp>
# include <string>
# include <sstream>
# include <fstream>
# include <unordered_map>
# include <vector>
# include <GL/glew.h>
# include <tuple>

# include "utility.hpp"
# include "resource.hpp" // struct element_resource

class material
{
public:
    using container_vnt = std::vector<utility::vertex_vnt>;
    using container_vn = std::vector<utility::vertex_vn>;
    using index_map = std::unordered_map<std::tuple<size_t, size_t, size_t>,
          unsigned, hash_ptr>;
    using vertices_idx = std::vector<unsigned>;
    void draw();
    glm::vec4& get_ambient();
    glm::vec4& get_specular();
    glm::vec4& get_diffuse();
    float& get_shininess();
    float& get_dissolve();
    GLuint& get_ambient_map_id();
    GLuint& get_diffuse_map_id();
    GLuint& get_specular_map_id();
    GLuint& get_dissolve_map_id();
    GLuint& get_bump_map_id();
    GLuint& get_bump_id();
    element_resource& get_resource();
    vertices_idx& get_indices();
    index_map& get_idx_lut();

    container_vnt& get_vertices_vnt();
    container_vn& get_vertices_vn();
private:
    // Ns
    float shininess = 1.0f;
    // Ka
    glm::vec4 ambient;
    // Kd
    glm::vec4 diffuse;
    // Ks
    glm::vec4 specular;
    // ambient texture map ==> map_Ka
    GLuint ambient_map_id = 0;
    // diffuse texture map ==> map_Kd
    GLuint diffuse_map_id = 0;
    // specular texture map ==> map_Ks
    GLuint specular_map_id = 0;
    // dissolve texture map ==> map_d
    GLuint dissolve_map_id = 0;
    // Bump texture map ==> map_bump
    GLuint bump_map_id = 0;
    // Bump  ==> bump
    GLuint bump_id = 0;
    float dissolve = 1.0f;

    // Associative map_ of all indices of object to check.  If index already
    // exists it's id can be return, otherwise a new one is created
    index_map idx_lut;

    element_resource resource;
    // Associated indices to material
    vertices_idx indices;
    container_vnt vertices_vnt;
    container_vn vertices_vn;
};

#endif // MATERIAL_HPP
