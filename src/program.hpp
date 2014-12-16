#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <GL/glew.h>
# include <vector>

# include "texture_binder.hpp"
# include "utility.hpp"
# include "light.hpp"

class program
{
public:
    using container_vnt = std::vector<utility::vertex_vnt>;
    using container_vn = std::vector<utility::vertex_vn>;
    using lights = std::vector<light*>;
    // Creates a new program class from some previously compiled program shaders
    program(GLuint program_id, render_type type);
    void init();
    void use();
    render_type get_render_type();
    // Loads material data onto shader, and binds its textures.
    void bind_material(material& mat);
    // Use to bind a single light. Called multiple times in deferred rendering.
    template <class light_t>
    void bind_light(light_t& light, const glm::mat4& view_mat);
    void bind_mvp(const glm::mat4&& mvp_mat);
    void bind_mvp(const glm::mat4& mvp_mat);
    void bind_scene(const glm::mat4& model_mat, const glm::mat4& view_mat,
                    const glm::mat4& proj_mat);
    void bind_screen_dimension(int width, int height);
    ~program();
private:
    // Base function to load lights
    template <typename light_t>
    // view_mat is the camera space view_matricx
    void bind_shadow(light_t& light, const glm::mat4& view_mat);
    void bind_light(light& light);
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

#endif // PROGRAM_HPP
