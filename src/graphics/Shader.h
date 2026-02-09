#pragma once
#include <string>
#include <unordered_map>

#include "glm/fwd.hpp"

class Shader
{
private:
	struct ShaderProgramSource
	{
		std::string vertexSource;
		std::string fragmentSource;
	};
	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;
	unsigned int programID;
	std::unordered_map<std::string, int> uniformCache;
public:
	Shader(const std::string& fileName);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void DeleteProgram();
	inline unsigned int GetProgramID() const
	{
		return programID;
	}
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetMatrix4f(const std::string& name, const glm::mat4& matrix);
	void SetMatrix3f(const std::string& name, const glm::mat3& matrix);
	void SetUniform3v(const std::string& name, const glm::vec3& vector);

private:
	ShaderProgramSource ParseFile(const std::string& fileName);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShaderProgram(const std::string& vertexShaderSource, const std::string& framentShaderSource);
	int GetUniformLocation(const std::string& name);
};
