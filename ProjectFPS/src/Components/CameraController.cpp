#include "pch.h"
#include "CameraController.h"

#include "Rendering/Renderer.h"
#include "../Input.h"
#include "glm/gtx/string_cast.hpp"

CameraController* CameraController::currentCameraController = nullptr;

void CameraController::Start()
{
	GetGameObject()->GetTransform()->Rotation = glm::vec3(90, 0, 0);
	Input::MouseMoved += [this](glm::vec2 move) { OnMouseMoved(move); };
}

void CameraController::Update(float deltaTime)
{
	currentCameraController = this;

	camera.position = GetGameObject()->GetTransform()->Position;
	camera.rotation = GetGameObject()->GetTransform()->Rotation;

	camera.perspectiveProjectionData.aspectRatio = (float)Renderer::GetWindowWidth() / Renderer::GetWindowHeight();

	if (enableMoving)
		HandleMovement(deltaTime);
}

glm::mat4 CameraController::GetViewMatrix()
{
	auto t = GetGameObject()->GetTransform();
	camera.position = t->Position;
	camera.rotation = t->Rotation;

	return camera.GetViewMatrix();
}

void CameraController::OnMouseMoved(glm::vec2 mouseMove)
{
	HandleRotation(mouseMove.x, mouseMove.y);
}

void CameraController::HandleMovement(float deltaTime)
{
	const auto& transform = GetGameObject()->GetTransform();

	if (Input::GetKeys()[GLFW_KEY_W])
		transform->Position += transform->GetForwardVector() * moveSpeed * deltaTime;
	if (Input::GetKeys()[GLFW_KEY_S])
		transform->Position -= transform->GetForwardVector() * moveSpeed * deltaTime;

	if (Input::GetKeys()[GLFW_KEY_A])
		transform->Position -= transform->GetRightVector() * moveSpeed * deltaTime;
	if (Input::GetKeys()[GLFW_KEY_D])
		transform->Position += transform->GetRightVector() * moveSpeed * deltaTime;
}

void CameraController::HandleRotation(float xMove, float yMove)
{
	if (!enableRotating)
		return;

	auto transform = GetGameObject()->GetTransform();

	transform->Rotation.x += yMove * rotateSpeed;
	transform->Rotation.y += -xMove * rotateSpeed;

	transform->Rotation.x = fminf(fmaxf(transform->Rotation.x, glm::radians(-80.0f)), glm::radians(80.0f));
}
