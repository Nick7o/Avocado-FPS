#pragma once

#include "Component.h"

#include "glm/fwd.hpp"
#include "glm/gtx/quaternion.hpp"

class Collider : public Component
{
public:
	virtual bool HasPointInside(glm::vec3 point) = 0;
	virtual bool Intersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& intersectionPoint) = 0;
};

class BoxCollider : public Collider
{
public:
	BoxCollider();
	BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 rot);

	virtual bool HasPointInside(glm::vec3 point) override;
	virtual bool Intersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& intersectionPoint) override;

	void SetPosition(glm::vec3 position);
	void SetSize(glm::vec3 size);
	void SetRotation(glm::vec3 eulerAngles);

	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetSize() { return size; }
	glm::quat GetRotation() { return rotation; }

	glm::mat4 GetMatrix() { return M; }
	glm::mat4 GetInverseMatrix() { return inverseM; }

private:
	void RebuildMatrix();

	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 size = glm::vec3(1.0f);
	glm::quat rotation = glm::quat(glm::vec3(0.0f));

	glm::mat4 M;
	glm::mat4 inverseM;
};
