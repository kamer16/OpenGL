#ifndef MATERIAL_HPP
# define MATERIAL_HPP

# include <string>
# include <sstream>
# include <glm/glm.hpp>

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
};

class material_lib
{
public:
    // The material library using filename in the string stream
    void load_material_lib(std::istringstream& iss);
};

#endif // MATERIAL_HPP
