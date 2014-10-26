#include "material.hpp"
#include "utility.hpp"
#include "texture_manager.hpp"

#include <string>
#include <fstream>
#include <iostream>

material_lib::material_lib(std::string&& dir)
    : dir_(dir)
{
}

void material_lib::update_material(material_ptr mtl, std::string& token)
{
    using namespace utility;

    if (!token.compare("map_Ka"))
        mtl->ambient_map = dir_ + unix_file(iss_);
    else if (!token.compare("map_Kd")) {
        std::string kd_name = unix_file(iss_);
        mtl->diffuse_map = dir_ + kd_name;
        mtl->diffuse_map_id = tm_.load_texture(dir_ + kd_name, GL_TEXTURE0);
    }
    else if (!token.compare("map_Ks"))
        mtl->specular_map = dir_ + unix_file(iss_);
    else if (!token.compare("map_bump"))
        mtl->bump_map = dir_ + unix_file(iss_);
    else if (!token.compare("map_d"))
        mtl->dissolve_map = dir_ + unix_file(iss_);
    else if (!token.compare("bump"))
        mtl->bump = dir_ + unix_file(iss_);
    else if (!token.compare("Ka"))
        mtl->ambient = make_vec3(iss_, "ambiant_mat");
    else if (!token.compare("Kd"))
        mtl->diffuse = make_vec3(iss_, "diffuse_mat");
    else if (!token.compare("Ks"))
        mtl->specular = make_vec3(iss_, "specular_mat");
    else if (!token.compare("Ns"))
        mtl->shininess = make_float(iss_, "shininess");
    else if (!token.compare("d"))
        mtl->dissolve = make_float(iss_, "dissolve");
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
            update_material(mtl, token);

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

void material::bind()
{
    // TODO bind one texture for all objects of same type
    static GLuint tex = 0;
    // Only bind texture if it wasn't already previously bound
    if (tex != diffuse_map_id) {
        glBindTexture(GL_TEXTURE_2D, diffuse_map_id);
        tex = diffuse_map_id;
    }
}
