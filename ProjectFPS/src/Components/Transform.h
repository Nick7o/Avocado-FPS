#pragma once

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "../Component.h"

class Transform : public Component
{
public:
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);

	glm::vec3 FixedRotation = glm::vec3(0.0f);

	Transform() = default;
	Transform(const Transform&) = default;
	Transform(const glm::vec3& position)
		: Position(position) { }

	glm::mat4 GetLocalToWorldMatrix() const
	{
		return glm::translate(glm::mat4(1.0f), Position)
			* glm::toMat4(glm::quat(Rotation))
			* glm::scale(glm::mat4(1.0f), Scale) * glm::toMat4(glm::quat(FixedRotation));
	}

	glm::vec3 GetForwardVector() const
	{
		return glm::quat(Rotation) * glm::vec3(0, 0, -1);
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

