#include "GL/glew.h"

#include "Renderer.h"

void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const Shader& shader) const
{
	//Clear();
	vertexArray.Bind();
	indexBuffer.Bind();
	shader.Bind();
	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}
void Renderer::Clear() const
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Renderer::drawTest(const VertexArray& vertexArray, const VertexBuffer& vertex_buffer) const
{
	vertexArray.Bind();

	vertex_buffer.Bind();
	glDrawArrays(GL_TRIANGLES, 0, 36);
}