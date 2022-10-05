#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class CameraController;

enum class ProjectionType { Ortho, Perspective };

class Camera
{
public:
	struct ProjectionData
	{
		float nearPlane = 0.01f, farPlane = 100.0f;

		ProjectionData() {}
		ProjectionData(float nearPlane, float farPlane)
			: nearPlane(nearPlane), farPlane(farPlane) {}

		virtual glm::mat4 GetMatrix() const = 0;
	};

	struct PerspectiveProjectionData : ProjectionData
	{
		float fovVertical = 60.0f, aspectRatio = 1.0f;
		
		PerspectiveProjectionData() {}
		PerspectiveProjectionData(float fovVertical, float aspectRatio, float nearPlane, float farPlane)
			: ProjectionData(nearPlane, farPlane), fovVertical(fovVertical), aspectRatio(aspectRatio) { }

		glm::mat4 GetMatrix() const override { return glm::perspective(glm::radians(fovVertical), aspectRatio, nearPlane, farPlane); }
	};

	struct OrthographicProjectionData : ProjectionData
	{
		float left = -1.0f, right = 1.0f, bottom = -1.0f, top = 1.0f;

		OrthographicProjectionData() {}
		OrthographicProjectionData(float left, float right, float bottom, float top, float nearPlane, float farPlane)
			: ProjectionData(nearPlane, farPlane), left(left), right(right), bottom(bottom), top(top) { }

		glm::mat4 GetMatrix() const override { return glm::ortho(left, right, bottom, top, nearPlane, farPlane); }
	};

public:
	glm::mat4 GetProjectionMatrix() const;
	glm::mat4 GetViewMatrix() const;

	void SetProjection(PerspectiveProjectionData data);
	void SetProjection(OrthographicProjectionData data);

	static glm::mat4 CalculateViewMatrix(const glm::vec3& position, const glm::vec3& lookDir);

public:
	glm::vec3 position;
	glm::vec3 rotation;

	ProjectionType projectionType;

	PerspectiveProjectionData perspectiveProjectionData;
	OrthographicProjectionData orthoProjectionData;
};