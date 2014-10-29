#include "material_lib.hpp"
#include "utility.hpp"
#include "texture_manager.hpp"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include <string>
#include <fstream>
#include <iostream>

// TODO material_lib should not have a texture manager, a program has acces to a
// unique texturme_manager, and the material_lib can access this texture_manager
// via a program object.
material_lib::material_lib(std::string&& dir)
    : dir_(dir)
{
}

auto
material_lib::get_materials() -> materials
{
    return materials_;
}

void material_lib::update_material(material_ptr mtl, std::string& token,
                                   resource_manager& rm)
{
    using namespace utility;

    if (!token.compare("map_Ka"))
        mtl->get_ambient_map_id() = rm.load_texture(dir_ + unix_file(iss_), 0);
    else if (!token.compare("map_Kd"))
        mtl->get_diffuse_map_id() = rm.load_texture(dir_ + unix_file(iss_), 1);
    else if (!token.compare("map_Ks"))
        mtl->get_specular_map_id() = rm.load_texture(dir_ + unix_file(iss_), 2);
    else if (!token.compare("map_bump"))
        mtl->get_bump_map_id() = rm.load_texture(dir_ + unix_file(iss_), 3);
    else if (!token.compare("map_d"))
        mtl->get_dissolve_map_id() = rm.load_texture(dir_ + unix_file(iss_), 4);
    else if (!token.compare("bump"))
        mtl->get_bump_id() = rm.load_texture(dir_ + unix_file(iss_), 3);
    else if (!token.compare("Ka"))
        mtl->get_ambient() = glm::vec4(make_vec3(iss_, "ambiant_mat"), 1);
    else if (!token.compare("Kd"))
        mtl->get_diffuse() = glm::vec4(make_vec3(iss_, "diffuse_mat"), 1);
    else if (!token.compare("Ks"))
        mtl->get_specular() = glm::vec4(make_vec3(iss_, "specular_mat"), 1);
    else if (!token.compare("Ns"))
        mtl->get_shininess() = make_float(iss_, "shininess");
    else if (!token.compare("d"))
        mtl->get_dissolve() = make_float(iss_, "dissolve");
}

void
material_lib::load_material_lib(std::istringstream& iss, resource_manager& rm)
{
    std::string filename, token;
    iss >> filename;
    ifs_.open(dir_ + filename);
    if (!ifs_.good()) {
        std::cerr << "Unable loading material lib : " << dir_ + filename
                  << std::endl;
        return;
    }
    std::string buff;
    std::getline(ifs_, buff);
    material_ptr mtl = nullptr;
    while (!ifs_.eof()) {
        iss_.str(buff);
        iss_ >> token;
        if (!token.compare("newmtl")) {
            mtl = new material;
            iss_ >> token;
            materials_[token] = mtl;
        }
        else if (mtl)
            update_material(mtl, token, rm);

        std::getline(ifs_, buff);
        token.clear();
        iss_.clear();
    }
    // Use empty string as default material
    mtl = new material;
    materials_[""] = mtl;
    ifs_.close();
}

void
material_lib::dump()
{
    for (auto mat : materials_) {
        std::cout << mat.first << std::endl;
        material* ptr = mat.second;
        std::cout << "\tshininess"    << ptr->get_shininess() << std::endl;
        std::cout << "\tambient\t"; utility::print(ptr->get_ambient());
        std::cout << "\tdiffuse\t"; utility::print(ptr->get_diffuse());
        std::cout << "\tspecular\t"; utility::print(ptr->get_specular());
        std::cout << "\tambient_map\t"  << ptr->get_ambient_map_id() << std::endl;
        std::cout << "\tdiffuse_map\t"  << ptr->get_diffuse_map_id() << std::endl;
        std::cout << "\tspecular_map\t" << ptr->get_specular_map_id() << std::endl;
        std::cout << "\tbump_map\t"     << ptr->get_bump_map_id() << std::endl;
        std::cout << "\tbump\t"         << ptr->get_bump_id() << std::endl;
        std::cout << "\tmap_d\t"        << ptr->get_dissolve_map_id() << std::endl;
    }
}

material*
material_lib::get_material(std::string& material_name)
{
    auto it = materials_.find(material_name);
    if (it == materials_.end()) {
        std::cerr << "Unable to find material name : " << material_name
                  << std::endl;
        // Default material when none is found
        return materials_[""];
    }
    return (*it).second;
}
