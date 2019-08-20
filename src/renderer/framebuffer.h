#pragma once

#include "texture.h"

namespace Jam3D {

class FrameBuffer 
{
public:
    FrameBuffer();
    void AttachTexture(unsigned int textureRendererID) const;
    void Bind() const;
    void Unbind() const;

private:
    unsigned int m_RendererID;

};

}