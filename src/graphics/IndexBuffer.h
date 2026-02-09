#pragma once
class IndexBuffer
{
private:
	unsigned int renderId;
	unsigned int count;
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer() = default;
	~IndexBuffer();
	void Bind() const;
	void UnBind() const;
	unsigned int GetId() const;
	unsigned int GetCount() const;
};