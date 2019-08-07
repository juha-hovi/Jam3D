#include "vertexbuffer.h"

#include <GL/glew.h>

// Creates OpenGL buffer with vertex data.
// Input:
// 	  -data: pointer to vertex data
// 	  -size: size of data in bytes
VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

// Deletes the vertex buffer.
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

// Binds the vertex buffer.
void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

// Unbinds the vertex buffer.
void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::UpdateBuffer(unsigned int offset, unsigned int size, const void* data)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}