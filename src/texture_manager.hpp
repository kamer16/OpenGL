#ifndef TEXTURE_MANAGER
# define TEXTURE_MANAGER

# include <string>
# include <GL/glew.h>

namespace texture_manager
{
    GLuint load_texture(std::string&& file, GLenum texture_unit);
    void set_shader_uniforms(GLuint program_id);
}

#endif // TEXTURE_MANAGER
