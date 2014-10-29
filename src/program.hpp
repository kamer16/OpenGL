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
    template <typename T>
    void load_data(GLuint program_id, std::vector<T>& data, const char* name,
                   GLuint* buffer_id);
    // Loads material data onto shader, and binds its textures.
    void load_material(material& mat);
    program();
    template <typename T>
    void load_vertex_buffer(std::vector<T>& vertices, GLuint* vert_buffer_id);
    void set_vao_attribs(container_vnt& vertices, size_t stride);
private:
    texture_binder texture_binder_;
    GLuint program_id_;
    struct material_location
    {
        GLint specular;
        GLint diffuse;
        GLint ambient;
        GLint shininess;
    } material_location_;
};

#endif // PROGRAM_HPP
