#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

unsigned int rbo;
unsigned int fbo;
GLuint texture;
GLuint depthrenderbuffer;

void InitFrameBuffers()
{
    
    // Generate FrameBuffers
    glGenFramebuffers(1, &fbo);
    // Bind FrameBuffers
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // gen texture
    glGenTextures(1, &texture);
    // bind texture
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 300, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 512, 512);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);
}

void BindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
}
void UnbindFrameBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

#endif // !FRAMEBUFFERS_H
