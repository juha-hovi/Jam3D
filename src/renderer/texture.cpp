#include "texture.h"
#include "stb_image/stb_image.h"

#include <GL/glew.h>

#include <iostream>

namespace Jam3D {

Texture::Texture(const std::string& fp)
	: m_RendererID(0), m_FilePath(fp), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0), m_Type(GL_TEXTURE_2D)
{
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
	else
		std::cout << "Failed to create GL_TEXTURE_2D texture!" << std::endl;
}

Texture::Texture(int width, int height)
	: m_RendererID(0), m_FilePath(""), m_LocalBuffer(nullptr), m_Width(width), m_Height(height), m_BPP(0), m_Type(GL_TEXTURE_CUBE_MAP)
{
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

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) const 
{
	glActiveTexture(GL_TEXTURE0 + slot);
	if (m_Type == GL_TEXTURE_2D)
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
	else if (m_Type == GL_TEXTURE_CUBE_MAP)
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);
}

void Texture::Unbind() const
{
	if (m_Type == GL_TEXTURE_2D)
		glBindTexture(GL_TEXTURE_2D, 0);
	else if (m_Type == GL_TEXTURE_CUBE_MAP)
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

}