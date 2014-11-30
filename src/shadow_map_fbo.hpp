#ifndef SHADOW_MAP_FBO_HPP
# define SHADOW_MAP_FBO_HPP

# include <GL/glew.h>

class shadow_map_fbo
{
public:
    bool init(GLsizei width, GLsizei height);
    void bind_for_writing();
    void bind_for_reading();
private:
    GLuint fbo_;
    GLuint shadow_map_;
};

#endif // SHADOW_MAP_FBO_HPP
