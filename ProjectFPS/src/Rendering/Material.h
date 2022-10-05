#pragma once

#include "Shader.h"

#include <map>
#include <glm/fwd.hpp>

#include <Rendering/Texture.h>

class Material
{
public:
	Material() = default;
	Material(Shader* shader);

	Shader* shader;

	void Bind();
	void Unbind();

	bool GetBool(const std::string& name);
	void SetBool(const std::string& name, float value);
	inline bool HasBool(const std::string& name) { return IsInContainer(boolContainer, name); }

	float GetFloat(const std::string& name);
	void SetFloat(const std::string& name, float value);
	inline bool HasFloat(const std::string& name) { return IsInContainer(floatContainer, name); }

	int GetInt(const std::string& name);
	void SetInt(const std::string& name, int value);
	inline bool HasInt(const std::string& name) { return IsInContainer(intContainer, name); }

	glm::vec3 GetVec3(const std::string& name);
	void SetVec3(const std::string& name, glm::vec3 value);
	inline bool HasVec3(const std::string& name) { return IsInContainer(vec3Container, name); }

	glm::vec4 GetVec4(const std::string& name);
	void SetVec4(const std::string& name, glm::vec4 value);
	inline bool HasVec4(const std::string& name) { return IsInContainer(vec4Container, name); }

	glm::mat4 GetMatrix4(const std::string& name);
	void SetMatrix4(const std::string& name, glm::mat4 value);
	inline bool HasMatrix4(const std::string& name) { return IsInContainer(mat4Container, name); }

	Texture* GetTexture(const std::string& name);
	void SetTexture(const std::string& name, std::shared_ptr<Texture> value);
	inline bool HasTexture(const std::string& name) { return IsInContainer(textureContainer, name); }
	unsigned int GetTextureCount() { return textureContainer.size(); }

protected:
	std::map<std::string, bool> boolContainer;
	std::map<std::string, float> floatContainer;
	std::map<std::string, int> intContainer;
	std::map<std::string, glm::vec3> vec3Container;
	std::map<std::string, glm::vec4> vec4Container;
	std::map<std::string, glm::mat4> mat4Container;
	std::map<std::string, std::shared_ptr<Texture>> textureContainer;

	template <typename T>
	inline bool IsInContainer(const std::map<std::string, T>& container, const std::string& key)
	{
		return container.find(key) != container.end();
	}
};

