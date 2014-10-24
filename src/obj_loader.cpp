#include <fstream> // ifstream
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include "obj_loader.hpp"
#include "material.hpp"
#include "mesh_object.hpp"
#include "utility.hpp"

void
obj_loader::get_vertex(const char *str, s_vertex_idx &v_idx, size_t nb_vertices_)
{
    int v = 1;
    int t = 1;
    int n = 1;
    sscanf(str, "%d/%d/%d", &v, &t, &n);
    if (v < 0)
        v_idx.v = nb_vertices_ - static_cast<size_t>(-v) + 1;
    else
        v_idx.v = static_cast<size_t>(v);
    if (t < 0)
        v_idx.t = nb_vertices_ - static_cast<size_t>(-t) + 1;
    else
        v_idx.t = static_cast<size_t>(t);
    if (n < 0)
        v_idx.n = nb_vertices_ - static_cast<size_t>(-t) + 1;
    else
        v_idx.n = static_cast<size_t>(n);
}

void
obj_loader::add_indices(size_t nb_vertices_)
{
    std::string vertex;
    s_vertex_idx v_idx[3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1} };
    for (unsigned idx = 0; idx < 3; ++idx) {
        iss_ >> vertex;
        get_vertex(vertex.c_str(), v_idx[idx], nb_vertices_);

        indices_.push_back(v_idx[idx]);
    }
    if (iss_.fail())
        std::cerr << "A face must have at least 3 vertices_" << std::endl;
    iss_ >> vertex;
    // If face is composed of 4 vertices_, than create 2 triangles
    if (!iss_.fail()) {
        s_vertex_idx v = { 1, 1, 1 };
        get_vertex(vertex.c_str(), v, nb_vertices_);
        indices_.push_back(v);
        indices_.push_back(v_idx[0]);
        indices_.push_back(v_idx[2]);
    }

    iss_ >> vertex;
    if (!iss_.fail())
        std::cerr << "Faces with more than 4 vertices_ are not handled"
                  << std::endl;
}

// Print out vector in the mesh format
// Mesh will only contain triangles
void
obj_loader::print_results()
{
    std::cout.setf(std::ios::fixed);
    for (auto vertex : vertices_) {
        std::cout << "v " << vertex.x << " "<< vertex.y << " " << vertex.z
                  <<  std::endl;
    }
    for (auto vertex : text_coords_) {
        std::cout << "vt " << vertex.x << " "<< vertex.y << std::endl;
    }
    for (auto vertex : normals_) {
        std::cout << "vn " << vertex.x << " "<< vertex.y << " " << vertex.z
                  << std::endl;
    }
    unsigned counter = 0;
    for (auto vertex : indices_) {
        if (counter % 3 == 0)
            std::cout << "f ";
        ++counter;
        std::cout <<  vertex.v << "/"<< vertex.t << "/" << vertex.n << " ";
        if (counter % 3 == 0)
            std::cout << std::endl;

    }
}

// Vertices and Normals
void
obj_loader::index_object(container3 &out_v, container3 &out_n)
{
    for (unsigned i = 0; i < indices_.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices_[i].v - 1;
        size_t n_idx = indices_[i].n - 1;

        out_v.push_back(vertices_[v_idx]);
        out_n.push_back(normals_[n_idx]);
    }
}

void
obj_loader::compute_flat_shading(unsigned i, glm::vec3& cross)
{
    normals_.push_back(cross);
    indices_[i - 2].n = normals_.size();
    indices_[i - 1].n = normals_.size();
    indices_[i].n = normals_.size();
}

void
obj_loader::compute_smooth_shading(std::vector<float>& normals_count,
                                   size_t idx1, size_t idx2, size_t idx3,
                                   glm::vec3& cross,
                                   unsigned i)
{
    normals_[idx1] *= (normals_count[idx1] - 1) / normals_count[idx1];
    normals_[idx1] += cross / normals_count[idx1];
    normals_[idx2] *= (normals_count[idx2] - 1) / normals_count[idx2];
    normals_[idx2] += cross / normals_count[idx2];
    normals_[idx3] *= (normals_count[idx3] - 1) / normals_count[idx3];
    normals_[idx3] += cross / normals_count[idx3];
    indices_[i - 2].n = idx1 + 1;
    indices_[i - 1].n = idx2 + 1;
    indices_[i].n = idx3 + 1;
}

void
obj_loader::compute_normals(char flat_shading)
{
    std::vector<float> normals_count(vertices_.size());
    // Each vertex has a unique normal
    if (!flat_shading)
        normals_.resize(vertices_.size());

    for (unsigned i = 2; i < indices_.size(); i += 3) {
        size_t idx1 = indices_[i -  2].v - 1;
        size_t idx2 = indices_[i -  1].v - 1;
        size_t idx3 = indices_[i].v - 1;
        glm::vec3 v1 = vertices_[idx1];
        glm::vec3 v2 = vertices_[idx2];
        glm::vec3 v3 = vertices_[idx3];
        normals_count[idx1]++;
        normals_count[idx2]++;
        normals_count[idx3]++;
        glm::vec3 cross = glm::cross(v2 - v1, v3 - v1);
        if (fabs(cross.x) > 0.0001f || fabs(cross.y) > 0.0001f ||
            fabs(cross.z) > 0.0001f)
            cross = glm::normalize(cross);
        else
            std::cerr << "Undefined normal for triangular line\n";
        if (flat_shading)
            compute_flat_shading(i, cross);
        else
            compute_smooth_shading(normals_count, idx1, idx2, idx3, cross, i);
    }
}

// Vertices and Normals and Textures
void
obj_loader::index_object(container3 &out_v, container3 &out_n, container2 &out_t)
{
    for (unsigned i = 0; i < indices_.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices_[i].v - 1;
        size_t n_idx = indices_[i].n - 1;
        size_t t_idx = indices_[i].t - 1;

        out_v.push_back(vertices_[v_idx]);
        out_n.push_back(normals_[n_idx]);
        out_t.push_back(text_coords_[t_idx]);
    }
}

void
obj_loader::print_triangles()
{
    std::cout.setf(std::ios::fixed);
    if (vertices_.size() != normals_.size() || vertices_.size() != text_coords_.size())
        std::cerr << "Object loader generated different size objects" << std::endl;

    for (unsigned i = 0; i < vertices_.size(); ++i) {
        std::cout << vertices_[i].x << ", " << vertices_[i].y << ", "
                  << vertices_[i].z << "\t";
        std::cout << normals_[i].x << ", " << normals_[i].y << ", "
                  << normals_[i].z << "\t";
        std::cout << text_coords_[i].x << ", " << text_coords_[i].y << std::endl;
    }
}

auto
obj_loader::load_obj(std::string& file, container3 &out_v, container3 &out_n,
                     container2 &out_t) -> objects*
{
    objects* res = new objects();
    material_lib mat_lib;

    std::string token;
    ifs_.open(file);
    std::string buff;
    if (!ifs_.good()) {
        std::cerr << file << ": not found" << std::endl;
        return nullptr;
    }
    std::getline(ifs_, buff);
    object* obj = nullptr;
    while (!ifs_.eof()) {
        iss_.str(buff);
        iss_ >> token;

        if (!token.compare("v"))
            vertices_.push_back(utility::make_vec3(iss_, "Vertices"));
        else if (!token.compare("vt"))
            text_coords_.push_back(utility::make_vec2(iss_, "Texture_coord"));
        else if (!token.compare("vn"))
            normals_.push_back(utility::make_vec3(iss_, "Normals"));
        else if (!token.compare("f"))
            add_indices(vertices_.size());
        else if (!token.compare("mtllib")) {
            mat_lib.load_material_lib(iss_, file.substr(0, file.find_last_of('/') + 1));
        }
        else if (!token.compare("usemtl")) {
            obj = new mesh_object();
            obj->set_material(mat_lib.get_material(iss_));
        }

        std::getline(ifs_, buff);
        token.clear();
        iss_.clear();
    }
    if (normals_.size() == 0)
        compute_normals(0);
    if (vertices_.size() == 0)
        std::cerr << "File does not define any vertices\n";
    // Vertices and Normals
    if (normals_.size() != 0 && text_coords_.size() == 0)
        index_object(out_v, out_n);
    // Vertices and Normals and Textures
    else if (text_coords_.size() != 0)
        index_object(out_v, out_n, out_t);
    ifs_.close();
    return res;
}
