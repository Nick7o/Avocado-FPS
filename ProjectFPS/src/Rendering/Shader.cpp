#include "pch.h"
#include "Shader.h"

#include "Shadinclude.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#include <iostream>

#include <Rendering/Texture.h>

Shader::Shader(const std::string& identifier)
{
	programID = 0;

	this->identifier = identifier;
}

Shader::~Shader()
{
	glDeleteProgram(programID);

	loadedShaders.erase(std::remove(loadedShaders.begin(), loadedShaders.end(), this), loadedShaders.end());
}

void Shader::Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	Load(vertexShaderPath.c_str(), fragmentShaderPath.c_str());
}

void Shader::Load(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	std::string vertexCode, fragmentCode;
	//std::ifstream vertexShaderFile, fragmentShaderFile;

	//vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	//fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		/*vertexShaderFile.open(vertexShaderPath);
		fragmentShaderFile.open(fragmentShaderPath);

		std::stringstream vertexShaderStream, fragmentShaderStream;

		vertexShaderStream << vertexShaderFile.rdbuf();
		fragmentShaderStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vertexShaderStream.str();
		fragmentCode = fragmentShaderStream.str();*/

		vertexCode = Shadinclude::load(vertexShaderPath).c_str();
		fragmentCode = Shadinclude::load(fragmentShaderPath).c_str();
	}
	catch (std::ifstream::failure exception)
	{
		Debug::LogError("Shader", "Error occured when reading a file.");
	}

	if (Compile(vertexCode.c_str(), fragmentCode.c_str()))
	{
		loadedShaders.push_back(this);
	}
}

bool Shader::Compile(const char* vertexShaderCode, const char* fragmentShaderCode)
{
	unsigned int vertexID, fragmentID;
	int returnCode;
	char infoLog[1024];

	// Vertex shader compilation
	vertexID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexID, 1, &vertexShaderCode, NULL);
	glCompileShader(vertexID);

	glGetShaderiv(vertexID, GL_COMPILE_STATUS, &returnCode);
	if (returnCode != GL_TRUE)
	{
		glGetShaderInfoLog(vertexID, 1024, NULL, infoLog);
		Debug::LogError("Shader::Vertex", infoLog);
		return false;
	}

	// Fragment shader compilation
	fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentID, 1, &fragmentShaderCode, NULL);
	glCompileShader(fragmentID);

	glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &returnCode);
	if (returnCode != GL_TRUE)
	{
		glGetShaderInfoLog(fragmentID, 1024, NULL, infoLog);
		Debug::LogError("Shader::Fragment", infoLog);
		return false;
	}

	// Shader program
	programID = glCreateProgram();
	glAttachShader(programID, vertexID);
	glAttachShader(programID, fragmentID);

	// Setting attribute locations before linking
	glBindAttribLocation(programID, VertexAttributeLocation, "aPos");
	glBindAttribLocation(programID, NormalAttributeLocation, "aNormal");
	glBindAttribLocation(programID, TangentAttributeLocation, "aTangent");
	glBindAttribLocation(programID, ColorAttributeLocation, "aColor0");
	glBindAttribLocation(programID, UV0AttributeLocation, "aUV0");

	glLinkProgram(programID);

	glGetProgramiv(programID, GL_LINK_STATUS, &returnCode);
	if (returnCode != GL_TRUE)
	{
		glGetProgramInfoLog(programID, 1024, NULL, infoLog);
		Debug::LogError("Shader::Program Linking", infoLog);
		return false;
	}

	// After linking we can delete shaders
	glDeleteShader(vertexID);
	glDeleteShader(fragmentID);
	return true;
}

void Shader::Use()
{
	glUseProgram(programID);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetBool(const std::string& name, bool value) const
{
	glProgramUniform1i(programID, GetLocation(name), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
	glProgramUniform1i(programID, GetLocation(name), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glProgramUniform1f(programID, GetLocation(name), value);
}

void Shader::SetVec3(const std::string& name, glm::vec3 value) const
{
	glProgramUniform3fv(programID, GetLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, glm::vec4 value) const
{
	glProgramUniform4fv(programID, GetLocation(name), 1, glm::value_ptr(value));
}

void Shader::SetMatrix4(const std::string& name, glm::mat4 value) const
{
	glProgramUniformMatrix4fv(programID, GetLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

std::vector<Shader*> Shader::loadedShaders;

Shader* Shader::Find(const std::string& identifier)
{
	auto it = std::find_if(loadedShaders.begin(), loadedShaders.end(), [identifier](Shader* s) {
		return s->identifier == identifier;
		});

	return it != loadedShaders.cend() ? *it : nullptr;
}