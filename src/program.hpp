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
    // Initialises shader resources, such as texture uniforms.
    // Initializes program and sets it as the current program
    program(const char* vertex_shader, const char* fragment_shader);
    void init();
    void use();
    // Loads material data onto shader, and binds its textures.
    void bind_material(material& mat);
    void bind_scene_constants(const glm::mat4& view_mat, lights& lights);
    void bind_scene(const glm::mat4& model_mat, const glm::mat4& view_mat,
                    const glm::mat4& proj_mat, lights& lights);
    ~program();
private:
    void bind_light(light& light);
    void bind_light_pos(light& light, const glm::mat3& normal_mat);
    GLuint program_id_;
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
