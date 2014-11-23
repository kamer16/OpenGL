#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <GL/glew.h>
# include <vector>

# include "texture_binder.hpp"
# include "utility.hpp"
# include "light.hpp"

#include <glm/gtc/type_ptr.hpp>


class program
{
public:
    using container_vnt = std::vector<utility::vertex_vnt>;
    using container_vn = std::vector<utility::vertex_vn>;
    using lights = std::vector<light*>;
    // Initialises shader resources, such as texture uniforms.
    // Initializes program and sets it as the current program
    program(const char* vertex_shader, const char* fragment_shader,
            render_type type);
    void init();
    void use();
    render_type get_render_type();
    // Loads material data onto shader, and binds its textures.
    void bind_material(material& mat);
    // Use to bind a single light. Called multiple times in deferred rendering.
    template <class light_t>
    void bind_light(light_t& light, const glm::mat4& view_mat);
    void bind_mvp(const glm::mat4&& mvp_mat);
    void bind_scene(const glm::mat4& model_mat, const glm::mat4& view_mat,
                    const glm::mat4& proj_mat);
    void bind_screen_dimension(int width, int height);
    ~program();
private:
    GLuint program_id_;
    render_type render_type_;
    texture_binder texture_binder_;
    struct material_location
    {
        GLint ambient;
        GLint diffuse;
        GLint specular;
        GLint shininess;
    } material_location_;
};

template <class light_t>
void
program::bind_light(light_t& light, const glm::mat4& view_mat)
{
    GLint specular_idx = glGetUniformLocation(program_id_, "light.specular");
    glUniform4fv(specular_idx, 1, glm::value_ptr(light.get_specular()));

    GLint diffuse_idx = glGetUniformLocation(program_id_, "light.diffuse");
    glUniform4fv(diffuse_idx, 1, glm::value_ptr(light.get_diffuse()));

    using namespace glm;
    // create directional light
    GLint light_dir_idx = glGetUniformLocation(program_id_, "light.position");
    // When world moves, lights direction moves with it, therefore we multiply
    // it by a normal matrix.
    if (light_t::is_dir)
    glUniform4fv(light_dir_idx, 1,
                 value_ptr(normalize(view_mat * light.get_position())));
    else {
        glUniform4fv(light_dir_idx, 1, value_ptr(view_mat * light.get_position()));
    }
}


#endif // PROGRAM_HPP
