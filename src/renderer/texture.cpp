#include "texture.h"

#include <GL/glew.h>

namespace Jam3D {

Texture::Texture()
	: m_RendererID(0), m_Width(0), m_Height(0)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}


}