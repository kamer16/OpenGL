#include "program.hpp"
#include "shader.hpp"

program::program(GLuint program_id, render_type type)
    : program_id_(program_id),
      render_type_(type)
{
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
    using namespace glm;
    // TODO use unfirorm buffer objects, glBindBufferBase(GL_UNIFORM_BUFFER)
    // glNamedBufferSubDataEXT();
    if (render_type_ == render_type::material) {
        glUniform4fv(material_location_.specular, 1, value_ptr(mat.get_specular()));
        glUniform4fv(material_location_.diffuse, 1, value_ptr(mat.get_diffuse()));
        glUniform4fv(material_location_.ambient, 1, value_ptr(mat.get_ambient()));
        glUniform1f(material_location_.shininess, mat.get_shininess());
    }
    else if (render_type_ == render_type::color)
        glUniform4fv(material_location_.ambient, 1, value_ptr(mat.get_ambient()));
    else if (render_type_ == render_type::stencil)
    { /* Empty, Stencils only render vertices */ }
    else
        texture_binder_.bind_material(mat);
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

void program::bind_mvp(const glm::mat4& mvp_mat)
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

void
program::bind_light(light& light)
{
    GLint specular_idx = glGetUniformLocation(program_id_, "light.specular");
    glUniform4fv(specular_idx, 1, glm::value_ptr(light.get_specular()));

    GLint diffuse_idx = glGetUniformLocation(program_id_, "light.diffuse");
    glUniform4fv(diffuse_idx, 1, glm::value_ptr(light.get_diffuse()));
}


template <>
void
program::bind_light(dir_light& light, const glm::mat4& view_mat)
{
    bind_light(light);
    using namespace glm;
    // create directional light
    GLint light_dir_idx = glGetUniformLocation(program_id_, "light.position");
    // When world moves, lights direction moves with it, therefore we multiply
    // it by a normal matrix.
    glUniform4fv(light_dir_idx, 1,
                 value_ptr(normalize(view_mat * light.get_position())));
}

template <>
void
program::bind_light(pos_light& light, const glm::mat4& view_mat)
{
    bind_light(light);
    using namespace glm;
    GLint light_dir_idx = glGetUniformLocation(program_id_, "light.position");
    glUniform4fv(light_dir_idx, 1, value_ptr(view_mat * light.get_position()));

    GLint const_att_idx = glGetUniformLocation(program_id_, "light.const_att");
    glUniform1f(const_att_idx, light.get_const_att());

    GLint lin_att_idx = glGetUniformLocation(program_id_, "light.lin_att");
    glUniform1f(lin_att_idx, light.get_linear_att());

    GLint quad_att_idx = glGetUniformLocation(program_id_, "light.quad_att");
    glUniform1f(quad_att_idx, light.get_quadratic_att());
}

template <>
void
program::bind_light(spot_light& light, const glm::mat4& view_mat)
{
    bind_light(static_cast<pos_light&>(light), view_mat);

    GLint spot_exp_idx = glGetUniformLocation(program_id_, "light.spot_exp");
    glUniform1f(spot_exp_idx, light.get_spot_exponent());

    GLint spot_cut_idx = glGetUniformLocation(program_id_, "light.spot_cut");
    glUniform1f(spot_cut_idx, light.get_spot_cutoff());

    GLint spot_dir_idx = glGetUniformLocation(program_id_, "light.spot_dir");
    using namespace glm;
    vec4 spot_dir = vec4(light.get_spot_dir(), 0);
    glUniform3fv(spot_dir_idx, 1, value_ptr(normalize(view_mat * spot_dir)));
}
