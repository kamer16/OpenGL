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
    struct textures_ids
    {
        GLuint ambiant;
        GLuint diffuse;
        GLuint specular;
        GLuint bump;
    } cached_id_;
};

#endif // TEXTURE_BINDER_HPP
