#include "texturecubemap.h"

#include <GL/glew.h>

namespace Jam3D {

TextureCubeMap::TextureCubeMap(int width, int height)
{
    m_Width = width;
    m_Height = height;
    
	glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);

    for (int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, m_Width,
                    m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE); 
}

void TextureCubeMap::Bind(unsigned int slot) const 
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void TextureCubeMap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}