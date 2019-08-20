#include "framebuffer.h"

#include <GL/glew.h>

namespace Jam3D {

FrameBuffer::FrameBuffer()
{
    glGenFramebuffers(1, &m_RendererID);
}

void FrameBuffer::AttachTexture(unsigned int textureRendererID) const
{
    Bind();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, textureRendererID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    Unbind();
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}