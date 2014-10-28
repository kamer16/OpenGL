#include "material.hpp"
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
material_lib::material_lib(std::string&& dir, texture_manager& tm)
    : dir_(dir),
      tm_(tm)
{
}

GLuint
material::get_diffuse_texture()
{
    return diffuse_map_id;
}

void
material::draw(GLuint program_id)
{
    bind(program_id);
    glBindVertexArray(vao_id);
    int nb_elt = static_cast<int>(indices.size());
    glDrawElements(GL_TRIANGLES, nb_elt, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void
material::load_index_buffer()
{
    glGenBuffers(1, &index_buffer_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_id);
    long unsigned byte_size = sizeof (vertices_idx::value_type) * indices.size();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, byte_size, indices.data(), GL_STATIC_DRAW);
}

template <typename T>
void
material::load_vertex_buffer(std::vector<T>& vertices, GLuint* vert_buffer_id)
{
    glGenBuffers(1, vert_buffer_id);
    glBindBuffer(GL_ARRAY_BUFFER, *vert_buffer_id);
    long unsigned byte_size = sizeof (T) * vertices.size();
    glBufferData(GL_ARRAY_BUFFER, byte_size, vertices.data(), GL_STATIC_DRAW);

}

void
material::set_vao_attribs(GLuint program_id, bool has_text_coord, size_t stride)
{
    // Sets shaders attribute for vertices positions
    GLint pos_location = glGetAttribLocation(program_id, "in_position");
    glEnableVertexAttribArray(pos_location); // Matches layout (location = 0)
    glVertexAttribPointer(pos_location, 3, GL_FLOAT, GL_FALSE, stride, 0);

    // Sets shaders attribute for texture coordinates
    if (has_text_coord)
    {
        GLint uv_idx = glGetAttribLocation(program_id, "in_uv");
        if (uv_idx != -1) {
            glEnableVertexAttribArray(uv_idx); // Matches layout (location = 1)
            GLvoid *offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
            glVertexAttribPointer(uv_idx, 2, GL_FLOAT, GL_FALSE, stride,
                                  offset);
        }
    }
    // Sets shaders attribute for color (r, g, b)
    GLint norm_idx = glGetAttribLocation(program_id, "in_norm");
    if (norm_idx != -1) {
        glEnableVertexAttribArray(norm_idx); // Matches layout (location = 1)
        GLvoid *offset;
        if (has_text_coord)
            offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3) +
                                                 sizeof (glm::vec2));
        else
            offset = reinterpret_cast<GLvoid *> (sizeof (glm::vec3));
        glVertexAttribPointer(norm_idx, 3, GL_FLOAT, GL_FALSE, stride, offset);
    }
}

void
material::bind_indexed_vao(GLuint program_id)
{
    size_t stride;
    if (vertices_vtn.size()) {
        load_vertex_buffer(vertices_vtn, &vertex_buffer_id);
        stride = sizeof (utility::vertex_vtn);
    }
    else {
        load_vertex_buffer(vertices_vn, &vertex_buffer_id);
        stride = sizeof (utility::vertex_vn);
    }
    // Currently materials store vao_id, as I need a different index buffer for
    // each material hence, a specific vao to keep that state.
    glGenVertexArrays(1, &vao_id);
    glBindVertexArray(vao_id);
    set_vao_attribs(program_id, 1, stride);
    //glBindBuffer(GL_ARRAY_BUFFER, vert_buffer_id_);
    load_index_buffer();
    glBindVertexArray(0);
}

auto material::get_vertices_vtn() -> container_vtn&
{
    return vertices_vtn;
}

auto material::get_vertices_vn() -> container_vn&
{
    return vertices_vn;
}

glm::vec4&
material::get_ambient()
{
    return ambient;
}

glm::vec4&
material::get_specular()
{
    return specular;
}

glm::vec4&
material::get_diffuse()
{
    return diffuse;
}

float
material::get_shininess()
{
    return shininess;
}

auto
material_lib::get_materials() -> materials
{
    return materials_;
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
        mtl->ambient = glm::vec4(make_vec3(iss_, "ambiant_mat"), 1);
    else if (!token.compare("Kd"))
        mtl->diffuse = glm::vec4(make_vec3(iss_, "diffuse_mat"), 1);
    else if (!token.compare("Ks"))
        mtl->specular = glm::vec4(make_vec3(iss_, "specular_mat"), 1);
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

void material::bind(GLuint program_id)
{
    // TODO bind one texture for all objects of same type
    static GLuint tex = 0;
    // Only bind texture if it wasn't already previously bound
    if (tex != diffuse_map_id) {
        glBindTexture(GL_TEXTURE_2D, diffuse_map_id);
        tex = diffuse_map_id;
    }
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    GLint specular_idx = glGetUniformLocation(program_id, "material.specular");
    glUniform4fv(specular_idx, 1, glm::value_ptr(specular));

    GLint diffuse_idx = glGetUniformLocation(program_id, "material.diffuse");
    glUniform4fv(diffuse_idx, 1, glm::value_ptr(diffuse));

    GLint ambient_idx = glGetUniformLocation(program_id, "material.ambient");
    glUniform4fv(ambient_idx, 1, glm::value_ptr(ambient));

    GLint shininess_idx = glGetUniformLocation(program_id, "material.shininess");
    glUniform1f(shininess_idx, shininess);
}
