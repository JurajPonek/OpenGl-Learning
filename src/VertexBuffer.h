#pragma once

class VertexBuffer
{
private:
	unsigned int renderId;
public:
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();
	void Bind() const;
	void UnBind() const;
	inline unsigned int GetId() const
	{
		return renderId;
	}
};