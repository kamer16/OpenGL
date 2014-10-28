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

# include "texture_manager.hpp"
# include "utility.hpp"

struct material
{
    using container_vtn = std::vector<utility::vertex_vtn>;
    using container_vn = std::vector<utility::vertex_vn>;
    using index_map = std::unordered_map<std::tuple<size_t, size_t, size_t>,
          unsigned, hash_ptr>;
    void bind_indexed_vao(GLuint program_id);
    // Vertices can be updated by caller
    container_vtn& get_vertices_vtn();
    container_vn& get_vertices_vn();
    void bind(GLuint program_id);
    using vertices_idx = std::vector<unsigned>;
    // Ns
    float shininess = 1.0f;
    // Ka
    glm::vec4 ambient;
    // Kd
    glm::vec4 diffuse;
    // Ks
    glm::vec4 specular;
    // ambient texture map ==> map_Ka
    std::string ambient_map;
    // diffuse texture map ==> map_Kd
    std::string diffuse_map;
    // Texture id for diffuse_map;
    GLuint diffuse_map_id = 0;
    // specular texture map ==> map_Ks
    std::string specular_map;
    // Bump texture map ==> map_bump
    std::string bump_map;
    // Bump  ==> bump
    std::string bump;
    // dissolve map  ==> map_d
    std::string dissolve_map;
    float dissolve = 1.0f;

    // Associative map_ of all indices of object to check.  If index already
    // exists it's id can be return, otherwise a new one is created
    index_map map;

    GLuint vao_id;
    GLuint index_buffer_id;
    GLuint vertex_buffer_id;
    // Associated indices to material
    vertices_idx indices;
    container_vtn vertices_vtn;
    container_vn vertices_vn;
private:
    // TODO should be called by resour/program manager
    template <typename T>
    void load_vertex_buffer(std::vector<T>& vertices, GLuint* id);
    // TODO should be called by resour/program manager
    void load_index_buffer();
    void set_vao_attribs(GLuint program_id, bool has_text_coord, size_t stride);
};

class material_lib
{
public:
    using materials = std::unordered_map<std::string, material*>;
    using material_ptr = material*;
    material_lib(std::string&& dir);
    // The material library using filename in the string stream
    void load_material_lib(std::istringstream& iss);
    // Returns pointer to a material for a given name or nullptr when not found.
    material_ptr get_material(std::string& mat_name);
    materials get_materials();
    void dump();
private:
    void update_material(material_ptr mtl, std::string& token);
    materials materials_;
    std::istringstream iss_;
    std::ifstream ifs_;
    std::string dir_;
    texture_manager tm_;
};

#endif // MATERIAL_HPP
