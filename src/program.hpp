#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <GL/glew.h>
# include <vector>

# include "texture_binder.hpp"
# include "utility.hpp"

class program
{
public:
    using container_vnt = std::vector<utility::vertex_vnt>;
    using container_vn = std::vector<utility::vertex_vn>;
    // Initialises shader resources, such as texture uniforms.
    // Initializes program and sets it as the current program
    program(const char* vertex_shader, const char* fragment_shader);
    void init();
    void use();
    // Loads material data onto shader, and binds its textures.
    void bind_material(material& mat);
    GLuint get_program_id();
    ~program();
private:
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
