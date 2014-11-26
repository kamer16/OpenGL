#include "g_buffer.hpp"

#include <iostream>

g_buffer::g_buffer()
{
}

bool g_buffer::init(GLsizei width, GLsizei height)
{
    const size_t nb_textures = sizeof (textures_) / sizeof (GLuint);
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
    glGenTextures(nb_textures, textures_);
    glGenTextures(1, &depth_texture_);
    glGenTextures(1, &final_texture_);

    for (size_t i = 0; i < nb_textures; ++i) {
        glBindTexture(GL_TEXTURE_2D, textures_[i]);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB,
                     GL_FLOAT, NULL);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                               GL_TEXTURE_2D, textures_[i], 0);
    }
    ////////////////////////////
    //// BIND DEPTH BUFFER  ////
    ////////////////////////////
    glBindTexture(GL_TEXTURE_2D, depth_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, depth_texture_, 0);

    ////////////////////////////
    //// BIND DEPTH STENCIL ////
    ////////////////////////////
    glBindTexture(GL_TEXTURE_2D, final_texture_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_FLOAT,
                 NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
                           GL_TEXTURE_2D, final_texture_, 0);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FB error, status: 0x" << status << std::endl;
        return false;
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    return true;
}

void g_buffer::bind_for_reading()
{
    // When g_buffer is used for reading we want to write output in default FBO
    // as its the only way to render data on screen.
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    size_t nb_textures = sizeof (textures_) / sizeof (GLuint);
    for (size_t i = 0; i < nb_textures; ++i) {
        // Here is where we send fb's data to the shader
        glActiveTexture(GL_TEXTURE5 + i);
        glBindTexture(GL_TEXTURE_2D, textures_[i]);
    }
}

void g_buffer::bind_for_writing()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
}

void g_buffer::set_read_buffer(GBUFFER_TEXTURE_TYPE type)
{
    glReadBuffer(GL_COLOR_ATTACHMENT0 + type);
}

void g_buffer::blit(GLsizei width, GLsizei height)
{
    // Allow drawing to main fb
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    // Main fb needs to read current fb
    bind_for_reading();

    // Tell width buffer we are reading from
    set_read_buffer(GBUFFER_TEXTURE_TYPE_POSITION);
    glBlitFramebuffer(0, 0, width / 2, height / 2,
                      0, 0, width / 2, height / 2,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);

    set_read_buffer(GBUFFER_TEXTURE_TYPE_POSITION);
    glBlitFramebuffer(width / 2, height / 2, width, height,
                      width / 2, height / 2, width, height,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);

    set_read_buffer(GBUFFER_TEXTURE_TYPE_NORMAL);
    glBlitFramebuffer(width / 2, 0, width, height / 2,
                      width / 2, 0, width, height / 2,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);

    set_read_buffer(GBUFFER_TEXTURE_TYPE_DIFFUSE);
    glBlitFramebuffer(0, height / 2, width / 2, height,
                      0, height / 2, width / 2, height,
                      GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void g_buffer::start_frame()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
    // At each frame, we erase the old contents
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}

void g_buffer::bind_for_geom_pass()
{
    const size_t nb_textures = sizeof (textures_) / sizeof (GLuint);
    GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(nb_textures, draw_buffers);
}

void g_buffer::bind_for_light_pass()
{
    // During light pass, all data is writing to a single fb.  A local fb is
    // used instead of directly writing to screen because the latter does not
    // allow acces the the g_buffers stencil buffer.
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    size_t nb_textures = sizeof (textures_) / sizeof (GLuint);
    for (size_t i = 0; i < nb_textures; ++i) {
        // Here is where we send fb's data to the shader
        glActiveTexture(GL_TEXTURE5 + i);
        glBindTexture(GL_TEXTURE_2D, textures_[i]);
    }
}

void g_buffer::bind_for_stencil_pass()
{
    // Make sure that the stencil pass does not affect the final fb.
    glDrawBuffer(GL_NONE);
}

void g_buffer::final_pass(GLsizei width, GLsizei height)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
    glBlitFramebuffer(0, 0, width, height,
                      0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
