#include "Cube.h"

#include "graphics/Renderer.h"

Cube::Cube(glm::vec3 position) : m_position(position), m_model(1.0f)
{
	m_model = glm::translate(m_model, position);
	m_vertices = {
		// ===== zadna stena (-Z) =====
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 0
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // 1
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 2
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // 3

		// ===== predna stena (+Z) =====
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 4
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 5
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,// 6
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 7

		// ===== lava stena (-X) =====
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0f, 0.0f, 0.0f, // 8
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f,// 9
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f,//10
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,//11

		// ===== prava stena (+X) =====
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,//12
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,//13
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,//14
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//15

		 // ===== spodna stena (-Y) =====
		 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,//16
		  0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,//17
		  0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,//18
		 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,//19

		 // ===== horna stena (+Y) =====
		 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,//20
		  0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,//21
		  0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,//22
		 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,//23
	};
	m_indices = {
		// zadna stena
		0,  1,  2,
		2,  3,  0,

		// predna stena
		 4,  5,  6,
		 6,  7,  4,

		 // lava stena
		  8,  9, 10,
		 10, 11,  8,

		 // prava stena
		 12, 13, 14,
		 14, 15, 12,

		 // spodna stena
		 16, 17, 18,
		 18, 19, 16,

		 // horna stena
		 20, 21, 22,
		 22, 23, 20
	};
	m_vao = std::make_unique<VertexArray>();
	m_vbo = std::make_unique<VertexBuffer>(m_vertices.data(), m_vertices.size() * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(3, false);
	layout.Push<float>(2, false);
	layout.Push<float>(3, false);
	m_ibo = std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
	m_vao->AddVertexBuffer(*m_vbo, layout);
	m_vao->AddIndexBuffer(*m_ibo);
}

void Cube::Draw(const Renderer& renderer, const Shader& shader)
{
	shader.Bind();
	m_vao->Bind();
	renderer.draw(*m_vao, *m_ibo, shader);
}

void Cube::Translate(glm::vec3 translation)
{
	m_model = glm::translate(m_model, translation);
}

void Cube::Rotate(float rotationInDegrees, glm::vec3 axis)
{
	m_model = glm::rotate(m_model, glm::radians(rotationInDegrees), axis);
}

void Cube::Scale(glm::vec3 scale)
{
	m_model = glm::scale(m_model, scale);
}
void Cube::UpdateModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, pos);
	m_model = glm::rotate(m_model, glm::radians(rot.x), { 1, 0, 0 });
	m_model = glm::rotate(m_model, glm::radians(rot.y), { 0, 1, 0 });
	m_model = glm::rotate(m_model, glm::radians(rot.z), { 0, 0, 1 });
	m_model = glm::scale(m_model, scale);
}