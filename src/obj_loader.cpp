#include <fstream> // ifstream
#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <cassert>

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> /* lookAt, perspective */
#include <glm/gtc/type_ptr.hpp> /* value_ptr */

#include "obj_loader.hpp"
#include "material.hpp"
#include "utility.hpp"
#include "texture_manager.hpp"

void
obj_loader::get_vertex(std::string& str, s_vertex_idx &v_idx)
{
    // 0 is a reserved value, which is used to check if index for attributes
    // v,or n exists
    int v = 0;
    int t = 0;
    int n = 0;
    size_t pos1 = str.find_first_of('/');
    std::string first = str.substr(0, pos1);
    std::istringstream iss(first);
    iss >> v;

    if (pos1 != std::string::npos) {
        size_t pos2 = str.find_first_of('/', pos1 + 1);
        std::string second = str.substr(pos1 + 1, pos2 - pos1);
        iss.clear();
        iss.str(second);
        iss >> t;
        if (pos2 != std::string::npos) {
            std::string third = str.substr(pos2 + 1, str.size() - pos2);
            iss.clear();
            iss.str(third);
            iss >> n;
        }
    }
    //sscanf(str, "%d/%d/%d", &v, &t, &n);
    if (v < 0)
        v_idx.v = vertices_.size() - static_cast<size_t>(-v) + 1;
    else
        v_idx.v = static_cast<size_t>(v);
    if (t < 0)
        v_idx.t = text_coords_.size() - static_cast<size_t>(-t) + 1;
    else
        v_idx.t = static_cast<size_t>(t);
    if (n < 0)
        v_idx.n = normals_.size() - static_cast<size_t>(-n) + 1;
    else
        v_idx.n = static_cast<size_t>(n);
}

void
obj_loader::add_indices()
{
    std::string vertex;
    s_vertex_idx v_idx[3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1} };
    for (unsigned idx = 0; idx < 3; ++idx) {
        iss_ >> vertex;
        get_vertex(vertex, v_idx[idx]);

        indices_.push_back(v_idx[idx]);
    }
    if (iss_.fail())
        std::cerr << "A face must have at least 3 vertices_" << std::endl;
    iss_ >> vertex;
    // If face is composed of 4 vertices_, than create 2 triangles
    if (!iss_.fail()) {
        s_vertex_idx v = { 1, 1, 1 };
        get_vertex(vertex, v);
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
obj_loader::index_object(index_map& map, vertices_idx& out_idx,
                         container_vn& out_vn)
{
    using namespace std;
    for (unsigned i = 0; i < indices_.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices_[i].v - 1;
        size_t n_idx = indices_[i].n - 1;
        auto pair = map.insert(make_pair(make_tuple(v_idx, n_idx, 0),
                                         out_vn.size()));
        if (pair.second) {
            out_vn.push_back({ vertices_[v_idx], normals_[n_idx] });
        }
        out_idx.push_back(pair.first->second);
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
obj_loader::index_object(index_map& map, vertices_idx& out_idx,
                         container_vtn& out_vtn)
{
    using namespace std;
    for (unsigned i = 0; i < indices_.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices_[i].v - 1;
        size_t n_idx = indices_[i].n - 1;
        size_t t_idx = indices_[i].t - 1;
        auto pair = map.insert(make_pair(make_tuple(v_idx, n_idx, t_idx),
                                         out_vtn.size()));
        if (pair.second) {
            out_vtn.push_back({ vertices_[v_idx], text_coords_[t_idx], normals_[n_idx] });
        }
        out_idx.push_back(pair.first->second);
    }
}

void
obj_loader::print_triangles(container3& vertices, container2& text_coords,
                            container3& normals)
{
    std::cout.setf(std::ios::fixed);
    if (vertices.size() != normals.size() || vertices.size() != text_coords.size())
        std::cerr << "Object loader generated different size objects" << std::endl;

    for (unsigned i = 0; i < vertices.size(); ++i) {
        std::cout << vertices[i].x << ", " << vertices[i].y << ", "
                  << vertices[i].z << "\t";
        std::cout << normals[i].x << ", " << normals[i].y << ", "
                  << normals[i].z << "\t";
        std::cout << text_coords[i].x << ", " << text_coords[i].y << std::endl;
    }
}

void obj_loader::set_material_indices(material* mat)
{
    if (indices_[0].n == 0)
        compute_normals(0);
    if (vertices_.size() == 0)
        std::cerr << "File does not define any vertices\n";
    assert(indices_[0].n && "We always hove normals");
    // Vertices and Normals
    if (indices_[0].t == 0)
        index_object(mat->map, mat->indices, mat->get_vertices_vn());
    // Vertices and Normals and Textures
    else
        index_object(mat->map, mat->indices, mat->get_vertices_vtn());
}

auto
obj_loader::load_obj(std::string& file) -> object*
{
    material* current_mat = nullptr;
    texture_manager tm;
    material_lib mat_lib(file.substr(0, file.find_last_of('/') + 1), tm);

    std::string token;
    ifs_.open(file);
    std::string buff;
    if (!ifs_.good()) {
        std::cerr << file << ": not found" << std::endl;
        return nullptr;
    }
    std::getline(ifs_, buff);
    while (!ifs_.eof()) {
        using namespace utility;
        iss_.str(buff);
        iss_ >> token;

        if (!token.compare("v"))
            vertices_.push_back(make_vec3(iss_, "Vertices"));
        else if (!token.compare("vt"))
            text_coords_.push_back(make_vec2(iss_, "Text_coord"));
        else if (!token.compare("vn"))
            normals_.push_back(make_vec3(iss_, "Normals"));
        else if (!token.compare("f"))
            add_indices();
        else if (!token.compare("mtllib")) {
            mat_lib.load_material_lib(iss_);
        }
        else if (!token.compare("usemtl")) {
            // Update indices in current_material, and add new vertex_elements
            // to resulting object.
            if (current_mat)
                set_material_indices(current_mat);
            // we just finished unsing old indices and don't want to add them to
            // new geometry
            indices_.clear();
            std::string mat_name;
            iss_ >> mat_name;
            // new material at work
            current_mat = mat_lib.get_material(mat_name);
        }

        std::getline(ifs_, buff);
        token.clear();
        iss_.clear();
    }
    // Now that materials have been computed we can bind them to object
    object* res = new object();
    // Update last object that was added to materials object list
    if (!current_mat) {
        // Use default material since there were none
        current_mat = new material();
        // Check if mat already exists or not in resulting vector
        current_mat->ambient = glm::vec4(0.2, 0.2, 0.2, 1);
        current_mat->diffuse = glm::vec4(0.8, 0.8, 0.8, 1);
        current_mat->specular = glm::vec4(1.0, 1., 1., 1.);
        res->add_material(current_mat);
    }
    // load indices into current_material
    set_material_indices(current_mat);
    auto mats = mat_lib.get_materials();
    for (auto pair : mats)
        res->add_material(pair.second);
    ifs_.close();
    return res;
}
