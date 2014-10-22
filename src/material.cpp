#include "material.hpp"
#include <string>
#include <fstream>

void
material_lib::load_material_lib(std::istringstream& iss)
{
    std::string filename;
    iss >> filename;
    std::ifstream ifs(filename);
}
