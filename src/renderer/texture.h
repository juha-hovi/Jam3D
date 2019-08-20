#pragma once

namespace Jam3D {

class Texture
{
protected:
	unsigned int m_RendererID;
	int m_Width, m_Height;

public:
	Texture();
	~Texture();

	virtual void Bind(unsigned int slot = 0) const = 0;
	virtual void Unbind() const = 0;

	inline int GetWidth() const { return m_Width; };
	inline int GetHeight() const { return m_Height; };
	inline int GetRendererID() const { return m_RendererID; };
};

}