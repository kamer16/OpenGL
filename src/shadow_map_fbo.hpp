#ifndef SHADOW_MAP_FBO_HPP
# define SHADOW_MAP_FBO_HPP

# include <GL/glew.h>

class shadow_map_fbo
{
public:
    shadow_map_fbo(bool have_color_map = 1)
        : have_color_map_(have_color_map)
    { }
    bool init(GLsizei width, GLsizei height);
    void bind_for_writing();
    void bind_for_reading();
    void blit_to_screen(GLsizei width, GLsizei height);
private:
    GLuint fbo_;
    GLuint shadow_map_;
    GLuint color_map_;

    // Extra color_map to help debug
    void bind_color_map(GLsizei width, GLsizei height);
    bool have_color_map_;
};

#endif // SHADOW_MAP_FBO_HPP
