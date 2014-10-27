#ifndef TEXTURE_MANAGER
# define TEXTURE_MANAGER

# include <string>
# include <unordered_map>
# include <GL/glew.h>

// TODO ideally have a unique texture_manager that handles all tetxrue resources
class texture_manager
{
public:
    using textures = std::unordered_map<std::string, GLuint>;
    GLuint load_texture(std::string&& file, GLenum texture_unit);
    void set_shader_uniforms(GLuint program_id);
private:
    textures textures_;
};

#endif // TEXTURE_MANAGER
