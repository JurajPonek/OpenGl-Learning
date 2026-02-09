#pragma once
#include <vector>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int redererId;
	std::vector<VertexBuffer> vertexBufferObjects;
	std::vector<IndexBuffer> indexBufferObjects;
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
	void AddIndexBuffer(const IndexBuffer& indexBuffer);
private:
	static unsigned int GetSizeOfType(unsigned int type);
};
