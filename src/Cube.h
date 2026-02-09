#pragma once
#include <memory>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"

#include "graphics/VertexArray.h"

class Shader;
class Renderer;

class Cube
{
public:
	Cube(glm::vec3 position);
	void Draw(const Renderer& renderer, const Shader& shader);
	void Translate(glm::vec3 translation);
	void Rotate(float rotationInDegrees, glm::vec3 axis);
	void Scale(glm::vec3 scale);
	inline glm::mat4 GetModelMatrix() const
	{
		return m_model;
	}
	void UpdateModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
	inline const glm::vec3& GetPosition() const
	{
		return m_position;
	}

private:
	glm::vec3 m_position;
	glm::mat4 m_model;
	std::vector<float> m_vertices;
	std::vector<unsigned int> m_indices;
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vbo;
	std::unique_ptr<IndexBuffer> m_ibo;
};
