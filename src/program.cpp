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

void
program::bind_light(light& light)//, const glm::mat3& normal_mat)
{
    // create directional light
    GLint light_dir_idx = glGetUniformLocation(program_id_, "light.position");
    // When world moves, lights direction moves with it, therefore we multiply
    // it by a normal matrix. // TODO do this HERE and not in shader
    glUniform3fv(light_dir_idx, 1,
                 glm::value_ptr(glm::normalize(light.get_position())));

    GLint specular_idx = glGetUniformLocation(program_id_, "light.param.specular");
    glUniform4fv(specular_idx, 1, glm::value_ptr(light.get_specular()));

    GLint diffuse_idx = glGetUniformLocation(program_id_, "light.param.diffuse");
    glUniform4fv(diffuse_idx, 1, glm::value_ptr(light.get_diffuse()));

    GLint ambient_idx = glGetUniformLocation(program_id_, "light.param.ambient");
    glUniform4fv(ambient_idx, 1, glm::value_ptr(light.get_ambient()));

    // TODO, currently setting default global light here
    glm::vec4 g_ambient(0.2f, 0.2f, 0.2f, 1.0f);
    GLint g_ambient_idx = glGetUniformLocation(program_id_, "global_ambient");
    glUniform4fv(g_ambient_idx, 1, glm::value_ptr(g_ambient));
}


void
program::bind_model_view_matrix(const glm::mat4& model_mat,
                                const glm::mat4& view_mat,
                                const glm::mat4& proj_mat)
{
    GLint mvp_idx = glGetUniformLocation(program_id_, "mvp_mat");
    GLint normal_mat_idx = glGetUniformLocation(program_id_, "normal_mat");
    GLint view_mat_idx = glGetUniformLocation(program_id_, "view_mat");

    glm::mat4 model_view_mat = view_mat * model_mat;
    glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_view_mat)));
    glm::mat4 mvp_mat = proj_mat * model_view_mat;

    glUniformMatrix4fv(mvp_idx, 1, GL_FALSE, glm::value_ptr(mvp_mat));
    glUniformMatrix3fv(normal_mat_idx, 1, GL_FALSE, glm::value_ptr(normal_mat));
    // Shaders needs view matrix to transform each vertex to camera space
    // allowing the shader to compute the direction vector from the vertex to cam
    glUniformMatrix4fv(view_mat_idx, 1, GL_FALSE, glm::value_ptr(view_mat));
}

//void bind_constants()
//{
//    GLint view_mat_idx = glGetUniformLocation(program_id_, "view_mat");
//    // Shaders needs view matrix to transform each vertex to camera space
//    // allowing the shader to compute the direction vector from the vertex to cam
//    glUniformMatrix4fv(view_mat_idx, 1, GL_FALSE, glm::value_ptr(view_mat));
//}
