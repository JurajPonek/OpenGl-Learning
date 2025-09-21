#pragma once
#include <GL/glew.h>
#include <vector>

class VertexBufferLayout
{
private:
	struct VertexBufferLayoutData
	{
		unsigned int count;
		unsigned int type;
		bool normalized;
	};

	unsigned int stride;
	std::vector<VertexBufferLayoutData> vertexAttributes;

public:
	VertexBufferLayout()
		:stride(0)
	{
	}
	~VertexBufferLayout() = default;

	template<typename T>
	void Push(unsigned int count, bool normalized)
	{
		static_assert(false, "Unsupported type");
	}
	template<>
	void Push<float>(unsigned int count, bool normalized)
	{
		vertexAttributes.push_back({ count, GL_FLOAT, normalized });
		stride += count * sizeof(float);
	}
	template<>
	void Push<unsigned int>(unsigned int count, bool normalized)
	{
		vertexAttributes.push_back({ count, GL_UNSIGNED_INT, normalized });
		stride += count * sizeof(unsigned int);
	}
	inline const std::vector<VertexBufferLayoutData>& GetVertexAttributes() const
	{
		return vertexAttributes;
	}
	inline unsigned int GetStride() const
	{
		return stride;
	}
};
