#include <fstream> // ifstream
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#include "obj_loader.hpp"
#include "utility.hpp"

static void add_indices(std::vector<s_vertex_idx> &indices, std::istringstream &iss)
{
    std::string vertex;
    s_vertex_idx v_idx[3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0} };
    for (unsigned idx = 0; idx < 3; ++idx) {
        iss >> vertex;
        sscanf(vertex.c_str(), "%u/%u/%u", &v_idx[idx].v, &v_idx[idx].t,
               &v_idx[idx].n);
        indices.push_back(v_idx[idx]);
    }
    if (iss.fail())
        std::cerr << "A face must have at least 3 vertices" << std::endl;
    iss >> vertex;
    // If face is composed of 4 vertices, than create 2 triangles
    if (!iss.fail()) {
        s_vertex_idx v = { 0, 0, 0 };
        sscanf(vertex.c_str(), "%u/%u/%u", &v.v, &v.t, &v.n);
        indices.push_back(v);
        indices.push_back(v_idx[0]);
        indices.push_back(v_idx[2]);
    }

    iss >> vertex;
    if (!iss.fail())
        std::cerr << "Faces with more than 4 vertices are not handled"
                  << std::endl;
}

static void add_normals(std::vector<utility::vec3> &normals, std::istringstream &iss)
{
    utility::vec3 normal;
    iss >> normal.x>> normal.y >> normal.z;
    if (iss.fail())
        std::cerr << "Missing argument for normals" << std::endl;
    normals.push_back(normal);
}

static void add_texture_coords(std::vector<utility::vec2> &text_coords,
                               std::istringstream &iss)
{
    utility::vec2 texture;
    iss >> texture.x >> texture.y;
    if (iss.fail())
        std::cerr << "Missing argument for texture coords" << std::endl;
    text_coords.push_back(texture);
}

static void add_vertices(std::vector<utility::vec3> &vertices, std::istringstream &iss)
{
    utility::vec3 vertex;
    iss >> vertex.x >> vertex.y >> vertex.z;
    if (iss.fail())
        std::cerr << "Missing argument for texture coords" << std::endl;
    vertices.push_back(vertex);
}


// Print out vector in the mesh format
// Mesh will only contain triangles
void print_results(std::vector<utility::vec3> &vertices,
                   std::vector<utility::vec3> &normals,
                   std::vector<utility::vec2> &text_coords,
                   std::vector<s_vertex_idx> &indices)
{
    std::cout.setf(std::ios::fixed);
    for (auto vertex : vertices) {
        std::cout << "v " << vertex.x << " "<< vertex.y << " " << vertex.z
                  <<  std::endl;
    }
    for (auto vertex : normals) {
        std::cout << "vn " << vertex.x << " "<< vertex.y << " " << vertex.z
                  << std::endl;
    }
    for (auto vertex : text_coords) {
        std::cout << "vt " << vertex.x << " "<< vertex.y << std::endl;
    }
    unsigned counter = 0;
    for (auto vertex : indices) {
        if (counter % 3 == 0)
            std::cout << "f ";
        ++counter;
        std::cout <<  vertex.v << "/"<< vertex.t << "/" << vertex.n << " ";
        if (counter % 3 == 0)
            std::cout << std::endl;

    }
}

static void index_object(std::vector<utility::vec3> &vertices,
                         std::vector<utility::vec3> &normals,
                         std::vector<utility::vec2> &text_coords,
                         std::vector<s_vertex_idx> &indices,
                         std::vector<utility::vec3> &out_v,
                         std::vector<utility::vec3> &out_n,
                         std::vector<utility::vec2> &out_t)
{
    for (unsigned i = 0; i < indices.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        unsigned v_idx = indices[i].v - 1;
        unsigned n_idx = indices[i].n - 1;
        unsigned t_idx = indices[i].t - 1;

        out_v.push_back(vertices[v_idx]);
        out_n.push_back(normals[n_idx]);
        out_t.push_back(text_coords[t_idx]);
    }
}
void
load_obj(const char *file,
         std::vector<utility::vec3> &out_v,
         std::vector<utility::vec3> &out_n,
         std::vector<utility::vec2> &out_t)
{
    std::vector<utility::vec3> vertices;
    std::vector<utility::vec3> normals;
    std::vector<utility::vec2> text_coords;
    std::vector<s_vertex_idx> indices;

    std::ifstream instr;
    std::string token;
    instr.open(file);
    std::string buff;
    if (!instr.good()) {
        std::cerr << file << ": not found" << std::endl;
        return;
    }
    std::getline(instr, buff);
    while (!instr.eof()) {
        std::istringstream iss(buff.c_str());
        iss >> token;

        if (!token.compare("v"))
            add_vertices(vertices, iss);
        else if (!token.compare("vt"))
            add_texture_coords(text_coords, iss);
        else if (!token.compare("vn"))
            add_normals(normals, iss);
        else if (!token.compare("f"))
            add_indices(indices, iss);

        std::getline(instr, buff);
        token.clear();
    }
    index_object(vertices, normals, text_coords, indices, out_v, out_n, out_t);
}
