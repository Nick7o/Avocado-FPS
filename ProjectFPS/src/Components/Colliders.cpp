#include "pch.h"
#include "Colliders.h"

#include "glm/glm.hpp"

BoxCollider::BoxCollider()
{
	RebuildMatrix();
}

BoxCollider::BoxCollider(glm::vec3 pos, glm::vec3 size, glm::vec3 rot)
{
	this->position = pos;
	this->size = size;
	this->rotation = rot;
	RebuildMatrix();
}

bool BoxCollider::HasPointInside(glm::vec3 point)
{
	Debug::LogError("BoxCollider", "HasPointInside is not implemented.");
	return false;
}

bool BoxCollider::Intersect(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3& intersectionPoint)
{
	float tMin = 0.0f, tMax = 9999999.0f;

	glm::vec3 halfSize = size * 0.5f;
	glm::vec3 minAABB = -halfSize;
	glm::vec3 maxAABB = halfSize;

	glm::vec3 delta = position - rayOrigin;

	for (int i = 0; i < 3; i++)
	{
		glm::vec3 axis = glm::normalize(glm::vec3(M[i].x, M[i].y, M[i].z));
		float e = glm::dot(axis, delta);
		float f = glm::dot(rayDirection, axis);

		if (fabs(f) > 0.001f)
		{
			float t1 = (e + minAABB[i]) / f;
			float t2 = (e + maxAABB[i]) / f;

			if (t1 > t2)
				std::swap(t1, t2);

			if (t2 < tMax) tMax = t2;
			if (t1 > tMin) tMin = t1;

			if (tMax < tMin)
				return false;
		}
		else if (-e + minAABB[i] > 0.0f || -e + maxAABB[i] < 0.0f)
			return false;
	}

	if (tMin < 0.0f)
		return false;

	intersectionPoint = rayOrigin + tMin * rayDirection;
	return true;
}

void BoxCollider::SetPosition(glm::vec3 position)
{
	this->position = position;
	RebuildMatrix();
}

void BoxCollider::SetSize(glm::vec3 size)
{
	this->size = size;
	RebuildMatrix();
}

void BoxCollider::SetRotation(glm::vec3 eulerAngles)
{
	this->rotation = glm::quat(eulerAngles);
	RebuildMatrix();
}

void BoxCollider::RebuildMatrix()
{
	M = glm::translate(glm::mat4(1.0f), position)
		* glm::toMat4(glm::quat(rotation))
		* glm::scale(glm::mat4(1.0f), size);

	inverseM = glm::inverse(M);
}