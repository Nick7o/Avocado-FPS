#pragma once

#include "GL/glew.h"

#include <string>
#include <vector>
#include <glm/fwd.hpp>

constexpr unsigned int VertexAttributeLocation = 0;
constexpr unsigned int NormalAttributeLocation = 1;
constexpr unsigned int ColorAttributeLocation = 2;
constexpr unsigned int UV0AttributeLocation = 3;
constexpr unsigned int TangentAttributeLocation = 4;

class Shader
{
public:
	std::string identifier;

	Shader(const std::string& identifier);
	~Shader();

	void Load(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void Load(const char* vertexShaderPath, const char* fragmentShaderPath);

	void Use();
	void Unbind();

	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec3(const std::string& name, glm::vec3 value) const;
	void SetVec4(const std::string& name, glm::vec4 value) const;
	void SetMatrix4(const std::string& name, glm::mat4 value) const;

	inline uint32_t GetLocation(const std::string& name) const { return glGetUniformLocation(programID, name.c_str()); }

	static Shader* Find(const std::string& name);
	static Shader* GetErrorShader() { return Find("__ERROR_SHADER"); }

protected:
	friend class Renderer;
	bool Compile(const char* vertexShaderCode, const char* fragmentShaderCode);

	unsigned int programID = 0;

	static std::vector<Shader*> loadedShaders;
};

