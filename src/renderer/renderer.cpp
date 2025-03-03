#include "renderer.h"

namespace Jam3D {

// Clears the screen.
void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Input:
// 	  -va: reference to a VertexArray object
// 	  -ib: reference to IndexBuffer object
// 	  -shader: reference to Shader object
void Renderer::Draw(GLenum type, const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	glDrawElements(type, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

}