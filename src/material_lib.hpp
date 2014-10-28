#ifndef MATERIAL_LIB_HPP
# define MATERIAL_LIB_HPP

# include <glm/glm.hpp>
# include <string>
# include <sstream>
# include <fstream>
# include <unordered_map>
# include <vector>
# include <GL/glew.h>
# include <tuple>

# include "utility.hpp"
# include "material.hpp"

// TODO material should not know about texture_manager,
// A program will contain a texture_manager and delegate textures ids to a
// material
class texture_manager;

class material_lib
{
public:
    using materials = std::unordered_map<std::string, material*>;
    using material_ptr = material*;
    material_lib(std::string&& dir, texture_manager& tm);
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
    texture_manager& tm_;
};

#endif // MATERIAL_LIB_HPP
