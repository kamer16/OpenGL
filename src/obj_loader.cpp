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
#include "utility.hpp"

static void get_vertex(const char *str, s_vertex_idx &v_idx, size_t nb_vertices)
{
    int v = 1;
    int t = 1;
    int n = 1;
    sscanf(str, "%d/%d/%d", &v, &t, &n);
    if (v < 0)
        v_idx.v = nb_vertices - static_cast<size_t>(-v) + 1;
    else
        v_idx.v = static_cast<size_t>(v);
    if (t < 0)
        v_idx.t = nb_vertices - static_cast<size_t>(-t) + 1;
    else
        v_idx.t = static_cast<size_t>(t);
    if (n < 0)
        v_idx.n = nb_vertices - static_cast<size_t>(-t) + 1;
    else
        v_idx.n = static_cast<size_t>(n);
}

static void add_indices(std::vector<s_vertex_idx> &indices, size_t nb_vertices,
                        std::istringstream &iss)
{
    std::string vertex;
    s_vertex_idx v_idx[3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1} };
    for (unsigned idx = 0; idx < 3; ++idx) {
        iss >> vertex;
        get_vertex(vertex.c_str(), v_idx[idx], nb_vertices);

        indices.push_back(v_idx[idx]);
    }
    if (iss.fail())
        std::cerr << "A face must have at least 3 vertices" << std::endl;
    iss >> vertex;
    // If face is composed of 4 vertices, than create 2 triangles
    if (!iss.fail()) {
        s_vertex_idx v = { 1, 1, 1 };
        get_vertex(vertex.c_str(), v, nb_vertices);
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
                   std::vector<utility::vec2> &text_coords,
                   std::vector<utility::vec3> &normals,
                   std::vector<s_vertex_idx> &indices)
{
    std::cout.setf(std::ios::fixed);
    for (auto vertex : vertices) {
        std::cout << "v " << vertex.x << " "<< vertex.y << " " << vertex.z
                  <<  std::endl;
    }
    for (auto vertex : text_coords) {
        std::cout << "vt " << vertex.x << " "<< vertex.y << std::endl;
    }
    for (auto vertex : normals) {
        std::cout << "vn " << vertex.x << " "<< vertex.y << " " << vertex.z
                  << std::endl;
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

// Vertices and Normals
static void
index_object(std::vector<utility::vec3> &vertices,
             std::vector<utility::vec3> &normals,
             std::vector<s_vertex_idx> &indices,
             std::vector<utility::vec3> &out_v,
             std::vector<utility::vec3> &out_n)
{
    for (unsigned i = 0; i < indices.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices[i].v - 1;
        size_t n_idx = indices[i].n - 1;

        out_v.push_back(vertices[v_idx]);
        out_n.push_back(normals[n_idx]);
    }
}

// Vertices
static void
index_object(std::vector<utility::vec3> &vertices,
             std::vector<s_vertex_idx> &indices,
             std::vector<utility::vec3> &out_v,
             std::vector<utility::vec3> &out_n,
             std::vector<utility::vec2> &out_t)
{
    for (unsigned i = 0; i < indices.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices[i].v - 1;

        out_v.push_back(vertices[v_idx]);
        // Default texture is vertex value
        // TODO make caller check for empty textures
        out_t.push_back({0, 0});
        if (i % 3 == 2) {
          glm::vec3 v1 = glm::vec3(out_v[i - 2].x, out_v[i - 2].y, out_v[i - 2].z);
          glm::vec3 v2 = glm::vec3(out_v[i - 1].x, out_v[i - 1].y, out_v[i - 1].z);
          glm::vec3 v3 = glm::vec3(out_v[i].x, out_v[i].y, out_v[i].z);

          glm::vec3 cross = glm::cross(v2 - v1, v3 - v1);
          if (fabs(cross.x) > 0.0001f || fabs(cross.y) > 0.0001f ||
              fabs(cross.z) > 0.0001f)
              cross = glm::normalize(cross);
          else
              std::cerr << "Undefined normal for triangular line\n";
          out_n.push_back({ cross.x, cross.y, cross.z });
          out_n.push_back({ cross.x, cross.y, cross.z });
          out_n.push_back({ cross.x, cross.y, cross.z });
        }
    }
}

// Vertices and Textures
static void
index_object(std::vector<utility::vec3> &vertices,
             std::vector<utility::vec2> &text_coords,
             std::vector<s_vertex_idx> &indices,
             std::vector<utility::vec3> &out_v,
             std::vector<utility::vec3> &out_n,
             std::vector<utility::vec2> &out_t)
{
    for (unsigned i = 0; i < indices.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices[i].v - 1;
        size_t t_idx = indices[i].t - 1;

        out_v.push_back(vertices[v_idx]);
        out_t.push_back(text_coords[t_idx]);
        if (i % 3 == 2) {
          glm::vec3 v1 = glm::vec3(out_v[i - 2].x, out_v[i - 2].y, out_v[i - 2].z);
          glm::vec3 v2 = glm::vec3(out_v[i - 1].x, out_v[i - 1].y, out_v[i - 1].z);
          glm::vec3 v3 = glm::vec3(out_v[i].x, out_v[i].y, out_v[i].z);

          glm::vec3 cross = glm::cross(v2 - v1, v3 - v1);
          if (fabs(cross.x) > 0.0001f || fabs(cross.y) > 0.0001f ||
              fabs(cross.z) > 0.0001f)
              cross = glm::normalize(cross);
          else
              std::cerr << "Undefined normal for triangular line\n";
          out_n.push_back({ cross.x, cross.y, cross.z });
          out_n.push_back({ cross.x, cross.y, cross.z });
          out_n.push_back({ cross.x, cross.y, cross.z });
        }
    }
}

// Vertices and Normals and Textures
static void
index_object(std::vector<utility::vec3> &vertices,
             std::vector<utility::vec3> &normals,
             std::vector<utility::vec2> &text_coords,
             std::vector<s_vertex_idx> &indices,
             std::vector<utility::vec3> &out_v,
             std::vector<utility::vec3> &out_n,
             std::vector<utility::vec2> &out_t)
{
    for (unsigned i = 0; i < indices.size(); ++i) {

        // Our base idx is 0, not 1 like in the mesh files
        size_t v_idx = indices[i].v - 1;
        size_t n_idx = indices[i].n - 1;
        size_t t_idx = indices[i].t - 1;

        out_v.push_back(vertices[v_idx]);
        out_n.push_back(normals[n_idx]);
        out_t.push_back(text_coords[t_idx]);
    }
}

void
print_trinagles(std::vector<utility::vec3> &vertices,
                std::vector<utility::vec3> &normals,
                std::vector<utility::vec2> &text_coords)
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
            add_indices(indices, vertices.size(), iss);

        std::getline(instr, buff);
        token.clear();
    }
    if (vertices.size() == 0)
        std::cerr << "File does not define any vertices\n";
    // Vertices and Normals
    if (normals.size() != 0 && text_coords.size() == 0)
        index_object(vertices, normals, indices, out_v, out_n);
    // Vertices and Textures
    else if (normals.size() == 0 && text_coords.size() != 0)
        index_object(vertices, text_coords, indices, out_v, out_n, out_t);
    // Vertices and Normals and Textures
    else if (text_coords.size() != 0)
        index_object(vertices, normals, text_coords, indices, out_v, out_n, out_t);
    else
    // Vertices
        index_object(vertices, indices, out_v, out_n, out_t);
}
