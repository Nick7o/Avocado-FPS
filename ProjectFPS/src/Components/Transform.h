#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "../Component.h"

class Transform : public Component
{
public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 fixedRotation = glm::vec3(0.0f);

	Transform() = default;
	Transform(const Transform&) = default;
	Transform(const glm::vec3& position)
		: position(position) { }

	glm::mat4 GetLocalToWorldMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), position)
			* glm::toMat4(glm::quat(rotation))
			* glm::scale(glm::mat4(1.0f), scale) * glm::toMat4(glm::quat(fixedRotation));
	}

	glm::vec3 GetForwardVector() const
	{
		return glm::quat(rotation) * glm::vec3(0, 0, -1);
	}

	glm::vec3 GetRightVector() const
	{
		return glm::normalize(glm::cross(GetForwardVector(), glm::vec3(0, 1, 0)));
	}

	glm::vec3 GetUpVector() const
	{
		return glm::normalize(glm::cross(GetRightVector(), GetForwardVector()));
	}
};

