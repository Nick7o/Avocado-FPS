#include "pch.h"
#include "Camera.h"

#include "glm/gtc/quaternion.hpp"

glm::mat4 Camera::GetProjectionMatrix() const
{
	if (projectionType == ProjectionType::Ortho)
		return orthoProjectionData.GetMatrix();
	else
		return perspectiveProjectionData.GetMatrix();
}

glm::mat4 Camera::GetViewMatrix() const
{
	return CalculateViewMatrix(position, glm::quat(rotation) * glm::vec3(0, 0, -1));
}

void Camera::SetProjection(PerspectiveProjectionData data)
{
	projectionType = ProjectionType::Perspective;
	perspectiveProjectionData = data;
}

void Camera::SetProjection(OrthographicProjectionData data)
{
	projectionType = ProjectionType::Ortho;
	orthoProjectionData = data;
}

glm::mat4 Camera::CalculateViewMatrix(const glm::vec3& position, const glm::vec3& lookDir)
{
	return glm::lookAt(position, position + lookDir, glm::vec3(0, 1, 0));
}
