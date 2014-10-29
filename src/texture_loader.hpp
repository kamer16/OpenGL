#ifndef TEXTURE_LOADER_HPP
# define TEXTURE_LOADER_HPP

# include <unordered_map>
# include <GL/glew.h>
# include <string>

class texture_loader
{
public:
    using textures = std::unordered_map<std::string, GLuint>;
    GLuint load_texture(std::string&& file, unsigned texture_unit);
private:
    textures textures_;
};

#endif // TEXTURE_LOADER_HPP
