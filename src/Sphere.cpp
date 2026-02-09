#include "Sphere.h"

#include <cmath>
#include <numbers>

#include "glm/vec3.hpp"
#include "glm/ext/quaternion_transform.hpp"

Sphere::Sphere(unsigned int xSegments = 36, unsigned int ySegments = 18)
	:m_model(1.0f)
{
	for (unsigned int y = 0; y <= ySegments; y++)
	{
		for (unsigned int x = 0; x <= xSegments; x++)
		{
			float xSegment = static_cast<float>(x) / static_cast<float>(xSegments);
			float ySegment = static_cast<float>(y) / static_cast<float>(ySegments);
			float xPos = cos(xSegment * std::numbers::pi * 2) * std::sin(ySegment * std::numbers::pi);
			float yPos = cos(ySegment * std::numbers::pi);
			float zPos = sin(xSegment * std::numbers::pi * 2) * std::sin(ySegment * std::numbers::pi);

			m_positions.push_back(xPos);
			m_positions.push_back(yPos);
			m_positions.push_back(zPos);

			m_positions.push_back(xPos);
			m_positions.push_back(yPos);
			m_positions.push_back(zPos);

			m_positions.push_back(xSegment);
			m_positions.push_back(ySegment);
		}
	}

	for (int y = 0; y < ySegments; y++)
	{
		for (int x = 0; x < xSegments; x++)
		{
			m_indices.push_back(y * (xSegments + 1) + x);
			m_indices.push_back((y + 1) * (xSegments + 1) + x);
			m_indices.push_back(y * (xSegments + 1) + x + 1);

			m_indices.push_back(y * (xSegments + 1) + x + 1);
			m_indices.push_back((y + 1) * (xSegments + 1) + x);
			m_indices.push_back((y + 1) * (xSegments + 1) + x + 1);
		}
	}

	m_vao = std::make_unique<VertexArray>();
	m_vao->Bind();
	m_vbo = std::make_unique<VertexBuffer>(m_positions.data(), m_positions.size() * sizeof(float));
	VertexBufferLayout layout;
	layout.Push<float>(3, false);
	layout.Push<float>(3, false);
	layout.Push<float>(2, false);

	m_ibo = std::make_unique<IndexBuffer>(m_indices.data(), m_indices.size());
	m_vao->AddVertexBuffer(*m_vbo, layout);
	m_vao->AddIndexBuffer(*m_ibo);
}

void Sphere::Draw(const Renderer& renderer, const Shader& shader)
{
	m_vao->Bind();
	shader.Bind();
	renderer.draw(*m_vao, *m_ibo, shader);
}

void Sphere::Translate(glm::vec3 translation)
{
	m_model = glm::translate(m_model, translation);
}

void Sphere::UpdateModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, pos);
	m_model = glm::rotate(m_model, glm::radians(rot.x), glm::vec3(1, 0, 0));
	m_model = glm::rotate(m_model, glm::radians(rot.y), glm::vec3(0, 1, 0));
	m_model = glm::rotate(m_model, glm::radians(rot.z), glm::vec3(0, 0, 1));
	m_model = glm::scale(m_model, scale);
}