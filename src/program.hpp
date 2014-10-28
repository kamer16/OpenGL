#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <GL/glew.h>
# include <vector>
# include <memory>

# include "texture_manager.hpp"

class program
{
public:
    template <typename T>
    void load_data(GLuint program_id, std::vector<T>& data, const char* name,
                   GLuint* buffer_id);
    void load_material(material& mat);
private:
    using texture_manager_ptr = std::shared_ptr<texture_manager>;
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
