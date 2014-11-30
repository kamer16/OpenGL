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

    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                           GL_TEXTURE_2D, shadow_map_, 0);

    // This fbo only needs writting to the depth buffer not the color buffers
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

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
}

void
shadow_map_fbo::bind_for_reading()
{
    glActiveTexture(GL_TEXTURE9);
    glBindTexture(GL_TEXTURE_2D, shadow_map_);
}
