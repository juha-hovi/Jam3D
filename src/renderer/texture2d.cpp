#include "texture2d.h"
#include "log.h"

#include "stb_image/stb_image.h"

#include <GL/glew.h>

namespace Jam3D {

Texture2D::Texture2D(const std::string& fp, unsigned int type)
	: m_FilePath(fp), m_LocalBuffer(nullptr), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(m_FilePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);

	if (type == STRETCH)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	else if (type == TILE)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
	else
		Jam3D::Log::Warning(std::string("Failed to create GL_TEXTURE_2D texture from: " + fp));
}

void Texture2D::Bind(unsigned int slot) const 
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

}