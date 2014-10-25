#ifndef MATERIAL_HPP
# define MATERIAL_HPP

# include <glm/glm.hpp>
# include <string>
# include <memory>
# include <sstream>
# include <fstream>
# include <unordered_map>

struct material
{
    // Ns
    float shininess = 1.0f;
    // Ka
    glm::vec3 ambient;
    // Kd
    glm::vec3 diffuse;
    // Ks
    glm::vec3 specular;
    // ambient texture map ==> map_Ka
    std::string ambient_map;
    // diffuse texture map ==> map_Kd
    std::string diffuse_map;
    // specular texture map ==> map_Ks
    std::string specular_map;
    // Bump texture map ==> map_bump
    std::string bump_map;
    // Bump  ==> bump
    std::string bump;
    // dissolve map  ==> map_d
    std::string dissolve_map;
    float dissolve = 1.0f;
};

class material_lib
{
public:
    using materials = std::unordered_map<std::string, std::shared_ptr<material>>;
    material_lib(std::string&& dir);
    // The material library using filename in the string stream
    void load_material_lib(std::istringstream& iss);
    // Returns pointer to a material for a given name or nullptr when not found.
    std::shared_ptr<material> get_material(std::istringstream& iss);
    void dump();
private:
    materials materials_;
    std::istringstream iss_;
    std::ifstream ifs_;
    std::string dir_;
};

#endif // MATERIAL_HPP
