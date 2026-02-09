#pragma once
#include <string>

class Texture
{
public:

	Texture(const std::string& filename);
	Texture(const std::string& filename, const std::string& type);
	Texture() = default;
	Texture(const Texture& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;
	~Texture();

	void Bind(int slot = 0) const;
	void UnBind() const;
	inline  const std::string& GetType() const
	{
		return  m_type;
	}
	void SetPath(const std::string& path)
	{
		m_path = path;
	}
	inline  const std::string& GetPath() const
	{
		return m_path;
	}

private:
	unsigned int m_textureHandle;
	unsigned char* m_data;
	int m_width;
	int m_heigth;
	int m_nrChannels;
	std::string m_type;
	std::string m_path;
};
