#include <fstream> // ifstream
#include <iostream>
#include <sstream>
#include <vector>
#include <cassert>

#include "obj_loader.hpp"

struct s_vertex_idx
{
    // Vertex index
    unsigned v;
    // Texture index
    unsigned t;
    // Normal index
    unsigned n;
};

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

static void add_normals(std::vector<float> &normals, std::istringstream &iss)
{
    float x, y, z;
    iss >> x>> y >> z;
    if (iss.fail())
        std::cerr << "Missing argument for normals" << std::endl;
    normals.push_back(x);
    normals.push_back(y);
    normals.push_back(z);
}

static void add_texture_coords(std::vector<float> &text_coords,
                               std::istringstream &iss)
{
    float s, t, q;
    q = 0;
    iss >> s >> t;
    if (iss.fail())
        std::cerr << "Missing argument for texture coords" << std::endl;
    iss >> q;
    text_coords.push_back(s);
    text_coords.push_back(t);
    text_coords.push_back(q);
}

static void add_vertices(std::vector<float> &vertices, std::istringstream &iss)
{
    float x, y, z, w;
    w = 1;
    iss >> x >> y >> z;
    if (iss.fail())
        std::cerr << "Missing argument for texture coords" << std::endl;
    iss >> w;
    vertices.push_back(x);
    vertices.push_back(y);
    vertices.push_back(z);
    vertices.push_back(w);
}

void load_obj(const char *file)
{
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> text_coords;
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
}
