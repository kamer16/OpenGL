#include "material.hpp"
#include "utility.hpp"

#include <string>
#include <fstream>
#include <iostream>

material_lib::material_lib(std::string&& dir)
    : dir_(dir)
{
}

void
material_lib::load_material_lib(std::istringstream& iss)
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
    std::shared_ptr<material> mtl = nullptr;
    while (!ifs_.eof()) {
        iss_.str(buff);
        iss_ >> token;
        if (!token.compare("newmtl")) {
            mtl = std::make_shared<material>();
            iss_ >> token;
            materials_[token] = mtl;
        }
        else if (mtl) {
            if (!token.compare("map_Ka"))
                mtl->ambient_map = dir_ + utility::unix_file(iss_);
            else if (!token.compare("map_Kd"))
                mtl->diffuse_map = dir_ + utility::unix_file(iss_);
            else if (!token.compare("map_Ks"))
                mtl->specular_map = dir_ + utility::unix_file(iss_);
            else if (!token.compare("map_bump"))
                mtl->bump_map = dir_ + utility::unix_file(iss_);
            else if (!token.compare("map_d"))
                mtl->dissolve_map = dir_ + utility::unix_file(iss_);
            else if (!token.compare("bump"))
                mtl->bump = dir_ + utility::unix_file(iss_);
            else if (!token.compare("Ka"))
                mtl->ambient = utility::make_vec3(iss_, "ambiant_mat");
            else if (!token.compare("Kd"))
                mtl->diffuse = utility::make_vec3(iss_, "diffuse_mat");
            else if (!token.compare("Ks"))
                mtl->specular = utility::make_vec3(iss_, "specular_mat");
            else if (!token.compare("Ns"))
                mtl->shininess = utility::make_float(iss_, "shininess");
            else if (!token.compare("d"))
                mtl->dissolve = utility::make_float(iss_, "dissolve");
        }

        std::getline(ifs_, buff);
        token.clear();
        iss_.clear();
    }

    ifs_.close();
}

void
material_lib::dump()
{
    for (auto mat : materials_) {
        std::cout << mat.first << std::endl;
        std::shared_ptr<material> ptr = mat.second;
        std::cout << "\tshininess"    << ptr->shininess << std::endl;
        std::cout << "\tambient\t"; utility::print(ptr->ambient);
        std::cout << "\tdiffuse\t"; utility::print(ptr->diffuse);
        std::cout << "\tspecular\t"; utility::print(ptr->specular);
        std::cout << "\tambient_map\t"  << ptr->ambient_map << std::endl;
        std::cout << "\tdiffuse_map\t"  << ptr->diffuse_map << std::endl;
        std::cout << "\tspecular_map\t" << ptr->specular_map << std::endl;
        std::cout << "\tbump_map\t"     << ptr->bump_map << std::endl;
        std::cout << "\tbump\t"         << ptr->bump << std::endl;
        std::cout << "\tmap_d\t"        << ptr->dissolve_map << std::endl;
    }
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
