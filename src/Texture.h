#pragma once
#include <string>

class Texture
{
public:

	Texture(const std::string& filename);
	~Texture();

	void Bind(int slot = 0) const;
	void UnBind() const;

private:
	unsigned int m_textureHandle;
	unsigned char* m_data;
	int m_width;
	int m_heigth;
	int m_nrChannels;
};
