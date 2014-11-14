#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <GL/glew.h>
# include <vector>

# include "texture_binder.hpp"
# include "utility.hpp"
# include "light.hpp"

enum class program_type
{
    material,
    bump,
    bump_dissolve,
    dissolve,
    color,
    basic
};

class program
{
public:
    using container_vnt = std::vector<utility::vertex_vnt>;
    using container_vn = std::vector<utility::vertex_vn>;
    using lights = std::vector<light*>;
    // Initialises shader resources, such as texture uniforms.
    // Initializes program and sets it as the current program
    program(const char* vertex_shader, const char* fragment_shader,
            program_type type);
    void init();
    void use();
    program_type get_type();
    // Loads material data onto shader, and binds its textures.
    void bind_material(material& mat);
    void bind_lights(const glm::mat4& view_mat, lights& lights);
    void bind_scene(const glm::mat4& model_mat, const glm::mat4& view_mat,
                    const glm::mat4& proj_mat);
    ~program();
private:
    void bind_light(light& light, const glm::mat4& view_mat);
    GLuint program_id_;
    program_type type_;
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
