#include <GL/glew.h>
#include "VertexArray.h"
VertexArray::VertexArray()
{
	glGenVertexArrays(1, &redererId);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &redererId);
}

void VertexArray::Bind() const
{
	glBindVertexArray(redererId);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

void VertexArray::AddVertexBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout)
{
	vertexBuffer.Bind();
	Bind();
	const auto& vertexAttributes = layout.GetVertexAttributes();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < vertexAttributes.size(); i++)
	{
		const auto& vertexAttribute = vertexAttributes[i];
		glVertexAttribPointer(i, vertexAttribute.count, vertexAttribute.type,
			(vertexAttribute.normalized ? GL_TRUE : GL_FALSE), layout.GetStride(), (const void*)offset);
		offset += vertexAttribute.count * GetSizeOfType(vertexAttribute.type);
		glEnableVertexAttribArray(i);
	}
	vertexBufferObjects.emplace_back(vertexBuffer);
}

void VertexArray::AddIndexBuffer(const IndexBuffer& indexBuffer)
{
	indexBuffer.Bind();
	indexBufferObjects.emplace_back(indexBuffer);
}

unsigned int VertexArray::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT: return sizeof(GLfloat);
	case GL_UNSIGNED_INT: return sizeof(GLuint);
	case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
	}
	return 0;
}