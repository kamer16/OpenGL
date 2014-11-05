#ifndef TEXTURE_BINDER_HPP
# define TEXTURE_BINDER_HPP

# include <GL/glew.h>
# include "material.hpp"

// Helper class for the program class.  Should only be used by the progrm class.
class texture_binder
{
public:
    void set_shader_uniforms(GLuint program_id);
    void bind_material(material& mat);
private:
    void bind_texture(GLenum texture_unit, GLuint new_tex, GLuint& cached_tex);
    // TODO get notified about which texture are to be updated
    struct textures_ids
    {
        GLuint ambient = static_cast<unsigned>(-1);
        GLuint diffuse = static_cast<unsigned>(-1);
        GLuint specular = static_cast<unsigned>(-1);
        GLuint dissolve = static_cast<unsigned>(-1);
        GLuint bump = static_cast<unsigned>(-1);
    } cached_id_;
};

#endif // TEXTURE_BINDER_HPP
