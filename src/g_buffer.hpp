#ifndef G_BUFFER_HPP
# define G_BUFFER_HPP

# include <GL/glew.h>

class g_buffer
{
public:
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_TEXCOORD,
        GBUFFER_NUM_TEXTURES
    };
    g_buffer();
    bool init(GLsizei width, GLsizei height);
    void bind_for_reading();
    void bind_for_writing();
    void set_read_buffer(GBUFFER_TEXTURE_TYPE type);
    // Copies g_buffer to main fram buffer
    void blit(GLsizei width, GLsizei height);
private:
    GLuint fbo_;
    GLuint depth_texture_;
    GLuint textures_[GBUFFER_NUM_TEXTURES];
};

#endif // G_BUFFER
