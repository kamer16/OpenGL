#include "material.hpp"

#include <string>
#include <fstream>

void
material_lib::load_material_lib(std::istringstream& iss)
{
    std::string filename;
    iss >> filename;
    std::ifstream ifs(filename);
    // TODO parse file
    ifs.close();
}

std::shared_ptr<material>
material_lib::get_material(std::istringstream& iss)
{
    std::string material_name;
    iss >> material_name;
    auto it = materials_.find(material_name);
    if (it == materials_.end())
        return nullptr;
    return (*it).second;
}
