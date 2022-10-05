#include "pch.h"
#include "Material.h"

#include "glm/glm.hpp"
#include "GL/glew.h"

Material::Material(Shader* shader)
{
	this->shader = shader;
}

void Material::Bind()
{
	if (shader == nullptr)
		return;

	shader->Use();

	for (auto val : boolContainer) shader->SetBool(val.first, val.second);
	for (auto val : floatContainer) shader->SetFloat(val.first, val.second);
	for (auto val : intContainer) shader->SetInt(val.first, val.second);
	for (auto val : vec3Container) shader->SetVec3(val.first, val.second);
	for (auto val : vec4Container) shader->SetVec4(val.first, val.second);
	for (auto val : mat4Container) shader->SetMatrix4(val.first, val.second);
	
	int textureId = 0;
	for (auto tex : textureContainer)
	{
		glActiveTexture(GL_TEXTURE0 + textureId);
		tex.second->Bind();
		shader->SetInt(tex.first, textureId);

		textureId++;
	}
}

void Material::Unbind()
{
	if (shader == nullptr)
		return;

	for (auto val : boolContainer) shader->SetBool(val.first, bool());
	for (auto val : floatContainer) shader->SetFloat(val.first, float());
	for (auto val : intContainer) shader->SetInt(val.first, int());
	for (auto val : vec3Container) shader->SetVec3(val.first, glm::vec3());
	for (auto val : vec4Container) shader->SetVec4(val.first, glm::vec4());
	for (auto val : mat4Container) shader->SetMatrix4(val.first, glm::mat4());

	int textureId = 0;
	for (auto tex : textureContainer)
	{
		glActiveTexture(GL_TEXTURE0 + textureId);
		//shader->SetInt(tex.first, 0);
		tex.second->Unbind();

		textureId++;
	}
	glActiveTexture(GL_TEXTURE0 + 0);

	shader->Unbind();
}

bool Material::GetBool(const std::string& name)
{
	auto it = boolContainer.find(name);
	return it != boolContainer.end() ? (*it).second : false;
}

void Material::SetBool(const std::string& name, float value)
{
	boolContainer[name] = value;
}

float Material::GetFloat(const std::string& name)
{
	auto it = floatContainer.find(name);
	return it != floatContainer.end() ? (*it).second : 0;
}

void Material::SetFloat(const std::string& name, float value)
{
	floatContainer[name] = value;
}

int Material::GetInt(const std::string& name)
{
	auto it = intContainer.find(name);
	return it != intContainer.end() ? (*it).second : 0;
}

void Material::SetInt(const std::string& name, int value)
{
	intContainer[name] = value;
}

glm::vec3 Material::GetVec3(const std::string& name)
{
	auto it = vec3Container.find(name);
	return it != vec3Container.end() ? (*it).second : glm::vec3();
}

void Material::SetVec3(const std::string& name, glm::vec3 value)
{
	vec3Container[name] = value;
}

glm::vec4 Material::GetVec4(const std::string& name)
{
	auto it = vec4Container.find(name);
	return it != vec4Container.end() ? (*it).second : glm::vec4();
}

void Material::SetVec4(const std::string& name, glm::vec4 value)
{
	vec4Container[name] = value;
}

glm::mat4 Material::GetMatrix4(const std::string& name)
{
	auto it = mat4Container.find(name);
	return it != mat4Container.end() ? (*it).second : glm::mat4();
}

void Material::SetMatrix4(const std::string& name, glm::mat4 value)
{
	mat4Container[name] = value;
}

Texture* Material::GetTexture(const std::string& name)
{
	auto it = textureContainer.find(name);
	return it != textureContainer.end() ? (*it).second.get() : nullptr;
}

void Material::SetTexture(const std::string& name, std::shared_ptr<Texture> value)
{
	if (value == nullptr)
	{
		if (HasTexture(name))
			textureContainer.erase(name);

		return;
	}

	textureContainer[name] = value;
}
