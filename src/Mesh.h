#pragma once
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

#include "Shader.h"
#include "VertexArray.h"

struct VertexData
{
	glm::vec3 Position = glm::vec3(.0f, .0f, .0f);
	glm::vec3 Normal = glm::vec3(.0f, .0f, .0f);
	glm::vec2 TexCoords = glm::vec2(.0f, .0f);
};

struct TextureData
{
	unsigned int id = 0;
	std::string type;
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<VertexData> vertices, std::vector<unsigned int> indices, std::vector<TextureData> textures);
	~Mesh();
	void Draw(Shader& shader);

private:

	void setupMesh();
private:
	std::vector<VertexData> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<TextureData>  m_textures;
	// render data
	unsigned int VAO, VBO, EBO;
};