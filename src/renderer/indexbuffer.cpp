#include "indexbuffer.h"

#include <GL/glew.h>

namespace Jam3D {

// Creates OpenGL buffer with index data.
// Input:
// 	  -data: pointer to index data
// 	  -count: number of data items
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

// Deletes the index buffer.
IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}

// Binds the index buffer.
void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

// Unbinds the index buffer.
void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

}