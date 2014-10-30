#include "program.hpp"
#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>

program::program(const char* vertex_shader, const char* fragment_shader)
{
    load_shaders(vertex_shader, fragment_shader, &program_id_);
}

void
program::bind_material(material& mat)
{
    texture_binder_.bind_material(mat);
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    using namespace glm;
    glUniform4fv(material_location_.specular, 1, value_ptr(mat.get_specular()));
    glUniform4fv(material_location_.diffuse, 1, value_ptr(mat.get_diffuse()));
    glUniform4fv(material_location_.ambient, 1, value_ptr(mat.get_ambient()));
    glUniform1f(material_location_.shininess, mat.get_shininess());
}

void
program::init()
{
    material_location_.ambient = glGetUniformLocation(program_id_,
                                                      "material.ambient");
    material_location_.diffuse = glGetUniformLocation(program_id_,
                                                      "material.diffuse");
    material_location_.specular = glGetUniformLocation(program_id_,
                                                       "material.specular");
    material_location_.shininess = glGetUniformLocation(program_id_,
                                                        "material.shininess");
    glUseProgram(program_id_);
    texture_binder_.set_shader_uniforms(program_id_);
}

program::~program()
{
    glDeleteProgram(program_id_);
}

void
program::use()
{
    glUseProgram(program_id_);
}

GLuint
program::get_program_id()
{
    return program_id_;
}
