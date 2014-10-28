#ifndef TEXTURE_MANAGER
# define TEXTURE_MANAGER

# include <string>
# include <unordered_map>
# include <GL/glew.h>

# include "material.hpp"

// TODO ideally have a unique texture_manager that handles all tetxrue resources
class texture_manager
{
public:
    using textures = std::unordered_map<std::string, GLuint>;
    GLuint load_texture(std::string&& file, unsigned texture_unit);
    void set_shader_uniforms(GLuint program_id);
    void bind_material(material& mat);
private:
    textures textures_;
    struct textures_ids
    {
        GLuint ambiant;
        GLuint diffuse;
        GLuint specular;
        GLuint bump;
    } cached_id_;
};

#endif // TEXTURE_MANAGER
