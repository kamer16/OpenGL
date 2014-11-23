#include "program.hpp"
#include "shader.hpp"

program::program(const char* vertex_shader, const char* fragment_shader,
                 render_type type)
    : render_type_(type)
{
    load_shaders(vertex_shader, fragment_shader, &program_id_);
}

void
program::bind_screen_dimension(int width, int height)
{
    use();
    GLint dims_idx = glGetUniformLocation(program_id_, "screen_size");
    glUniform2fv(dims_idx, 1, glm::value_ptr(glm::vec2(width, height)));
}

void
program::bind_material(material& mat)
{
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    if (render_type_ == render_type::material) {
        using namespace glm;
        glUniform4fv(material_location_.specular, 1, value_ptr(mat.get_specular()));
        glUniform4fv(material_location_.diffuse, 1, value_ptr(mat.get_diffuse()));
        glUniform4fv(material_location_.ambient, 1, value_ptr(mat.get_ambient()));
        glUniform1f(material_location_.shininess, mat.get_shininess());
    }
    else {
        texture_binder_.bind_material(mat);
    }
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
    use();
    texture_binder_.set_shader_uniforms(program_id_);
}

program::~program()
{
    glDeleteProgram(program_id_);
}

void
program::use()
{
    static GLuint cached_program_id_ = 0;
    if (program_id_ != cached_program_id_) {
        glUseProgram(program_id_);
        cached_program_id_ = program_id_;
    }
}


void program::bind_mvp(const glm::mat4&& mvp_mat)
{
    GLint mvp_idx = glGetUniformLocation(program_id_, "mvp_mat");
    glUniformMatrix4fv(mvp_idx, 1, GL_FALSE, glm::value_ptr(mvp_mat));
}

void
program::bind_scene(const glm::mat4& model_mat,
                    const glm::mat4& view_mat,
                    const glm::mat4& proj_mat)
{
    GLint mvp_idx = glGetUniformLocation(program_id_, "mvp_mat");
    GLint normal_mat_idx = glGetUniformLocation(program_id_, "normal_mat");

    glm::mat4 model_view_mat = view_mat * model_mat;
    glm::mat3 normal_mat = glm::transpose(glm::inverse(glm::mat3(model_view_mat)));
    glm::mat4 mvp_mat = proj_mat * model_view_mat;

    glUniformMatrix4fv(mvp_idx, 1, GL_FALSE, glm::value_ptr(mvp_mat));
    glUniformMatrix3fv(normal_mat_idx, 1, GL_FALSE, glm::value_ptr(normal_mat));

    GLint mv_idx = glGetUniformLocation(program_id_, "mv_mat");
    // Shaders needs model_view matrix to transform each vertex to camera space
    // allowing the shader to compute the direction vector from the vertex to cam
    glUniformMatrix4fv(mv_idx, 1, GL_FALSE, glm::value_ptr(model_view_mat));
}

render_type
program::get_render_type()
{
    return render_type_;
}
