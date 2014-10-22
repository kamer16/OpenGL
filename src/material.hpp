#ifndef MATERIAL_HPP
# define MATERIAL_HPP

# include <string>
# include <memory>
# include <sstream>
# include <glm/glm.hpp>

#include <unordered_map>

struct material
{
    // Ns
    float shininess;
    // Ka
    glm::vec3 ambient;
    // Kd
    glm::vec3 diffuse;
    // Ks
    glm::vec3 specular;
    // ambient texture map ==> map_Ka
    const char* ambient_map;
    // diffuse texture map ==> map_Kd
    const char* diffuse_map;
    // specular texture map ==> map_Ks
    const char* specular_map;
    // Bump texture map ==> map_bump
    const char* bump_map;
    // Bump  ==> bump
    const char* bump;
    float dissolve;
    float padding_;
};

class material_lib
{
public:
    using materials = std::unordered_map<std::string, std::shared_ptr<material>>;
    // The material library using filename in the string stream
    void load_material_lib(std::istringstream& iss);
    // Returns pointer to a material for a given name or nullptr when not found.
    std::shared_ptr<material> get_material(std::istringstream& iss);
private:
    materials materials_;
};

#endif // MATERIAL_HPP
