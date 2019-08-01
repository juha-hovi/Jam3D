#include "vertexarray.h"

#include <GL/glew.h>

// Creates OpenGL vertex array object.
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_RendererID)
}

// Deletes the vertex array object.
VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_RendererID);
}

// Adds a vertex buffer into the vertex array.
// Input:
// 	  -vb: reference to VertexBuffer object
// 	  -layout: reference to VertexBufferLayout object
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;

	for (int i = 0; i < elements.size(); ++i)
	{
		const auto& element = element[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferLayout::VertexBufferElement::GetSizeOfType(element.type);
	}
}

// Binds the vertex array object.
void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

// Unbinds the vertex array object.
void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}