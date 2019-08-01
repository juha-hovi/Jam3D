#pragma once

#include "vertexbuffer.h"
#include "vertexbufferlayout.h"

// Interface for OpenGL vertex array object
class VertexArray
{
private:
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};