#pragma once

#include <GL/glew.h>

#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

// Handles drawing and clearing the screen.
class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};