#pragma once
#include <memory>
#include <vector>

#include "Renderer.h"
#include "VertexArray.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"

class Sphere
{
public:
	Sphere(unsigned int xSegments, unsigned int ySegments);
	void Draw(const Renderer& renderer, const Shader& shader);
	void Translate(glm::vec3 translation);
	inline glm::mat4 GetModelMatrix() const { return m_model; }
	void UpdateModelMatrix(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
private:
	glm::mat4 m_model;
	std::vector<float> m_positions;
	std::vector<glm::vec2> m_uv;
	std::vector<unsigned int> m_indices;
	std::vector<glm::vec3> m_normals;
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<VertexBuffer> m_vbo;
	std::unique_ptr<IndexBuffer> m_ibo;
};
