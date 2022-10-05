#include "pch.h"
#include "Light.h"

#include "Transform.h"
#include "Rendering/Renderer.h"
#include "Shapes.h"
#include "CameraController.h"

std::shared_ptr<Camera> Light::GetCamera(int faceId)
{
	if (internalCameraRef == nullptr)
		internalCameraRef = std::make_shared<Camera>();

	auto t = GetGameObject()->GetTransform();
	internalCameraRef->position = t->position;
	internalCameraRef->rotation = t->rotation;

	switch (type)
	{
	case LightType::Directional:
		// todo: make it a bit less like this...
		CameraController* currentCam;
		currentCam = CameraController::GetCurrent();
		if (currentCam != nullptr)
			internalCameraRef->position = glm::ceil(currentCam->camera.position);

		internalCameraRef->position -= t->GetForwardVector() * 50.0f;

		internalCameraRef->SetProjection(Camera::OrthographicProjectionData(-50, 50, -50, 50, 0.01f, 100.0f));
		break;
	case LightType::Spot:
		internalCameraRef->SetProjection(Camera::PerspectiveProjectionData(glm::degrees(outerSpotAngle), 1.0f, 0.01f, 100.0f));
		break;
	case LightType::Point:
		internalCameraRef->SetProjection(Camera::PerspectiveProjectionData(glm::degrees(outerSpotAngle), 1.0f, 0.01f, 100.0f));
		break;
	default:
		break;
	}

	return internalCameraRef;
}

void Light::Start()
{

}

void Light::Update(float deltaTime)
{
	auto t = GetGameObject()->GetTransform();
	auto rot = t->rotation;
	//t->Rotation = glm::vec3(rot.x + deltaTime, 0, 0);

	//Renderer::DrawMesh(Shapes::CreateCube({0.1f,0.1f,0.5f}), nullptr, t->GetLocalToWorldMatrix(), CameraController::GetCurrent()->camera, false);
}