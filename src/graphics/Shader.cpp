#include <GL/glew.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& fileName)
	: programID(0), fragmentShaderID(0), vertexShaderID(0)
{
	ShaderProgramSource sources = ParseFile(fileName);
	vertexShaderID = CompileShader(sources.vertexSource, GL_VERTEX_SHADER);
	fragmentShaderID = CompileShader(sources.fragmentSource, GL_FRAGMENT_SHADER);
	programID = CreateShaderProgram(sources.vertexSource, sources.fragmentSource);
}

Shader::~Shader()
{
	DeleteProgram();
}

Shader::ShaderProgramSource Shader::ParseFile(const std::string& fileName)
{
	enum class ShaderType
	{
		NONE = -1, VERTEXSHADER = 0, FRAGMENTSHADER = 1
	};
	std::ifstream file(fileName);
	std::stringstream ss[2];
	std::string line;
	ShaderType shaderType = ShaderType::NONE;
	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				shaderType = ShaderType::VERTEXSHADER;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				shaderType = ShaderType::FRAGMENTSHADER;
			}
		}
		else
		{
			ss[(int)shaderType] << line << "\n";
		}
	}
	return ShaderProgramSource{ ss[0].str(), ss[1].str() };
}
unsigned int Shader::CompileShader(const std::string& source, unsigned int type)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, NULL);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int messageLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);
		std::vector<char> message(messageLength);
		glGetShaderInfoLog(id, messageLength, NULL, message.data());
		std::cout << "Error in " << (type == GL_VERTEX_SHADER ? "vertex shader" : "fragment shader") << std::endl;
		std::cout << message.data();
		glDeleteShader(id);
		return -1;
	}
	return id;
}
unsigned int Shader::CreateShaderProgram(const std::string& vertexShaderSource, const std::string& framentShaderSource)
{
	unsigned int id = glCreateProgram();
	glAttachShader(id, vertexShaderID);
	glAttachShader(id, fragmentShaderID);
	glLinkProgram(id);
	glValidateProgram(id);
	int result;
	glGetProgramiv(id, GL_LINK_STATUS, &result);
	if (!result)
	{
		int messageLength;
		glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength);
		std::vector<char> message(messageLength);
		glGetProgramInfoLog(id, messageLength, NULL, message.data());
		std::cout << "Error while Linking" << std::endl;
		std::cout << message.data() << std::endl;
		return -1;
	}
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	return id;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (uniformCache.find(name) != uniformCache.end())
	{
		return uniformCache.at(name);
	}
	int location = glGetUniformLocation(programID, name.c_str());
	if (location == -1)
		std::cout << "Warning uniform " << name << " does not exist!";

	uniformCache[name] = location;
	return location;
}

void Shader::Bind() const
{
	glUseProgram(programID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::DeleteProgram()
{
	glUseProgram(0);
	glDeleteProgram(programID);
}
void Shader::SetBool(const std::string& name, bool value)
{
	glUniform1i(GetUniformLocation(name), static_cast<int>(value));
}
void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}
void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}
void Shader::SetMatrix4f(const std::string& name, const glm::mat4& matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetMatrix3f(const std::string& name, const glm::mat3& matrix)
{
	glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetUniform3v(const std::string& name, const glm::vec3& vector)
{
	glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(vector));
}