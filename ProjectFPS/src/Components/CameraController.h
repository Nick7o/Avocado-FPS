#pragma once

#include <memory>

#include "Camera.h"
#include "Component.h"
#include "Transform.h"

class CameraController : public Component
{
public:
	void Start();
	void Update(float deltaTime);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix() const { return camera.GetProjectionMatrix(); }

	static CameraController* GetCurrent() { return currentCameraController; }

public:
	float moveSpeed = 5, rotateSpeed = 0.005f;
	Camera camera;

	bool enableMoving = true;
	bool enableRotating = true;

private:
	void HandleMovement(float deltaTime);
	void HandleRotation(float xMove, float yMove);

	void OnMouseMoved(glm::vec2 mouseMove);

	static CameraController* currentCameraController;
};