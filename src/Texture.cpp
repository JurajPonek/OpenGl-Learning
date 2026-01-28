#include <GL/glew.h>
#include "Texture.h"

#include <iostream>

#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filename) : m_textureHandle(0), m_data(nullptr), m_width(0), m_heigth(0), m_nrChannels(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_data = stbi_load(filename.c_str(), &m_width, &m_heigth, &m_nrChannels, STBI_rgb_alpha);
	if (m_data)
	{
		glGenTextures(1, &m_textureHandle);
		glBindTexture(GL_TEXTURE_2D, m_textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_heigth, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);
		this->UnBind();
		stbi_image_free(m_data);
	}
	else std::cout << "Error while loading texture";
}
Texture::~Texture()
{
	glDeleteTextures(1, &m_textureHandle);
}

void Texture::Bind(int slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_textureHandle);
}
void Texture::UnBind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}