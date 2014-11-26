#ifndef G_BUFFER_HPP
# define G_BUFFER_HPP

# include <GL/glew.h>

class g_buffer
{
public:
    enum GBUFFER_TEXTURE_TYPE {
        GBUFFER_TEXTURE_TYPE_POSITION,
        GBUFFER_TEXTURE_TYPE_NORMAL,
        GBUFFER_TEXTURE_TYPE_DIFFUSE,
        GBUFFER_TEXTURE_TYPE_SPECULAR,
        GBUFFER_NUM_TEXTURES
    };
    g_buffer();
    bool init(GLsizei width, GLsizei height);
    void bind_for_reading();
    void bind_for_writing();
    void set_read_buffer(GBUFFER_TEXTURE_TYPE type);
    // Function called before light shading.  It loads all the needed texture
    // for the light shading
    void bind_for_light_pass();
    // Function called before write all geometry data to textures.  It binds
    // multiple textures to allow MRT.
    void bind_for_geom_pass();
    // Function called when we start rendering lights from textures.  It load
    // framebuffer for writing
    void start_frame();
    // Copies g_buffer to main fram buffer
    void blit(GLsizei width, GLsizei height);
    // Before stencil pass, asserts that the final rendering fb will not be
    // affected
    void bind_for_stencil_pass();
    // Called to copy internal fb, to main's screen fb.
    void final_pass(GLsizei width, GLsizei height);
private:
    GLuint fbo_;
    GLuint depth_texture_;
    GLuint final_texture_;
    GLuint textures_[GBUFFER_NUM_TEXTURES];
};

#endif // G_BUFFER
