#ifndef MATERIAL_LIB_HPP
# define MATERIAL_LIB_HPP

# include <glm/glm.hpp>
# include <string>
# include <sstream>
# include <fstream>
# include <unordered_map>
# include <vector>
# include <memory>
# include <GL/glew.h>
# include <tuple>

# include "utility.hpp"
# include "material.hpp"
# include "resource_manager.hpp"

class material_lib
{
public:
    using materials = std::unordered_map<std::string, material*>;
    using material_ptr = material*;
    using resource_manager_ptr = std::shared_ptr<resource_manager>;
    material_lib(std::string&& dir);
    // The material library using filename in the string stream
    void load_material_lib(std::istringstream& iss, resource_manager_ptr rm);
    // Returns pointer to a material for a given name or nullptr when not found.
    material_ptr get_material(std::string& mat_name);
    materials get_materials();
    void dump();
private:
    void update_material(material_ptr mtl, std::string& token,
                         resource_manager_ptr rm);
    void add_material(material* mtl, std::string& material_name);
    materials materials_;
    std::istringstream iss_;
    std::ifstream ifs_;
    std::string dir_;
};

#endif // MATERIAL_LIB_HPP
