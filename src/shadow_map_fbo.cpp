#include "shadow_map_fbo.hpp"

#include <iostream>

bool
shadow_map_fbo::init(GLsizei width, GLsizei height)
{
    glGenFramebuffers(1, &fbo_);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
    glGenTextures(1, &shadow_map_);
    glBindTexture(GL_TEXTURE_2D, shadow_map_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, shadow_map_, 0);

    // This fbo only needs writting to the depth buffer not the color buffers
    if (have_color_map_)
        bind_color_map(width, height);
    else {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "FB error, status: 0x" << status << std::endl;
        return false;
    }
    return true;
}

void
shadow_map_fbo::bind_for_writing()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_);
    glClear(GL_DEPTH_BUFFER_BIT);

    if (have_color_map_) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawBuffer(GL_COLOR_ATTACHMENT0);
    }
}

void
shadow_map_fbo::bind_for_reading()
{
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, shadow_map_);
}

void shadow_map_fbo::bind_color_map(GLsizei width, GLsizei height)
{
    glGenTextures(1, &color_map_);
    glBindTexture(GL_TEXTURE_2D, color_map_);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB,
                 GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                           GL_TEXTURE_2D, color_map_, 0);
}

void shadow_map_fbo::blit_to_screen(GLsizei width, GLsizei height)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_);
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glBlitFramebuffer(0, 0, width, height,
                      0, 0, width / 4, height / 4, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}
