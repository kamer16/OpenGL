#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <GL/glew.h>
# include <vector>
# include <memory>

# include "texture_manager.hpp"
# include "utility.hpp"

class program
{
public:
    using texture_manager_ptr = std::shared_ptr<texture_manager>;
    using container_vnt = std::vector<utility::vertex_vnt>;
    using container_vn = std::vector<utility::vertex_vn>;
    template <typename T>
    void load_data(GLuint program_id, std::vector<T>& data, const char* name,
                   GLuint* buffer_id);
    void load_material(material& mat);
    program(texture_manager_ptr tm);
    const texture_manager_ptr& get_texture_manager();
    template <typename T>
    void load_vertex_buffer(std::vector<T>& vertices, GLuint* vert_buffer_id);
    void set_vao_attribs(bool has_text_coord, size_t stride);
private:
    texture_manager_ptr texture_manager_;
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
