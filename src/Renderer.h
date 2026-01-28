#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

class Renderer
{
private:
public:
	void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const;
	void Clear() const;
	void drawTest(const VertexArray& vertexArray, const VertexBuffer& vertex_buffer) const;
};
