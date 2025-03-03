#pragma once

namespace Jam3D {

// Interface for OpenGL buffer containing vertex data.
class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
	void UpdateBuffer(unsigned int offset, unsigned int size, const void* data);
};

}