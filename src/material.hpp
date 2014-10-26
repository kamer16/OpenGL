#ifndef MATERIAL_HPP
# define MATERIAL_HPP

# include <glm/glm.hpp>
# include <string>
# include <sstream>
# include <fstream>
# include <unordered_map>
# include <vector>
# include <GL/glew.h>

# include "texture_manager.hpp"
# include "object.hpp"

struct material
{
    void bind(GLuint program_id);
    using objects_t = std::vector<object*>;
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
    objects_t objects;
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
