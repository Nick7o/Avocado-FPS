#include "pch.h"
#include "Player.h"
#include "Enemy.h"

#include "Scene.h"
#include "Components/CollisionController.h"
#include "Components/Transform.h"
#include "Components/CameraController.h"

#include "Rendering/Renderer.h"
#include "Shapes.h"
#include "Input.h"

#include "glm/gtx/string_cast.hpp"

void Player::Start()
{
	Puppet::Start();

	cameraController = SceneManager::GetInstance().GetCurrentScene()->GetComponent<CameraController>();
	if (cameraController == nullptr)
		Debug::LogError("Player", "Camera controller not found.");
	else
		cameraController->enableMoving = false;

	auto lightGO = new GameObject();
	lightGO->GetTransform()->Position = { 0.0f, 0.1f, 0.0f };
	weaponFlashlight = lightGO->AddComponent(new Light());
	weaponFlashlight->color = { 0.25f, 0.05f, 1.0f };
	weaponFlashlight->type = LightType::Spot;
	weaponFlashlight->SetSpotAngleDegrees(15.0f, 35.0f);
	weaponFlashlight->intensity = 10.0f;

	// Events
	Input::MouseButtonChange += [this](int button, bool isDown) {
		if (button == GLFW_MOUSE_BUTTON_1 && isDown)
			Fire();
	};

	Input::KeyChange += [this](int key, bool isDown) {
		if (key == GLFW_KEY_E && isDown)
		{
			if (weaponFlashlight->intensity > 0.01f)
				weaponFlashlight->intensity = 0.0f;
			else
				weaponFlashlight->intensity = 8.0f;
		}
	};
}

void Player::Update(float deltaTime)
{
	auto t = GetGameObject()->GetTransform();

	if (cameraController != nullptr)
	{
		auto camTransform = cameraController->GetGameObject()->GetTransform();
		camTransform->Position = t->Position + glm::vec3(0, height, 0);

		t->Rotation = { 0, camTransform->Rotation.y, 0 };

		if (weapon != nullptr)
		{
			auto weaponExtraRot = glm::vec3{ glm::radians(180.0), glm::radians(-270.0), -glm::radians(90.0) };
			auto weaponOffset = glm::quat(camTransform->Rotation) * glm::vec3(0.015f, -0.0025f, -0.018f);
			weapon->GetTransform()->FixedRotation = weaponExtraRot;
			glm::vec3 newWeaponPos = camTransform->Position + weaponOffset;
			glm::vec3 newWeaponRot = camTransform->Rotation;

			// Weapon sway effect
			weapon->GetTransform()->Position = glm::mix(weapon->GetTransform()->Position + posChange, newWeaponPos, fminf(deltaTime * 15.0f, 1.0f));
			weapon->GetTransform()->Position.y = newWeaponPos.y;
			weapon->GetTransform()->Rotation = glm::mix(weapon->GetTransform()->Rotation, newWeaponRot, deltaTime * 6.0f);

			auto lightT = weaponFlashlight->GetGameObject()->GetTransform();
			lightT->Position = weapon->GetTransform()->Position + weapon->GetTransform()->GetForwardVector() * 0.1f;
			lightT->Rotation = weapon->GetTransform()->Rotation;
		}
	}

	HandleMovement(deltaTime);
	HandlePhysics(deltaTime);
	
	//Renderer::DrawMeshWireframe(Shapes::CreateCube(glm::vec3(0.02f, 0.02f, 1.5f)), nullptr,
	//	world, CameraController::GetCurrent()->camera);
}

void Player::Fire()
{
	auto camT = cameraController->GetGameObject()->GetTransform();
	auto rayOrigin = camT->Position;
	auto rayDirection = camT->GetForwardVector();

	weapon->GetTransform()->Rotation += glm::vec3(glm::radians(15.0f), 0, 0);

	HitInfo hitInfo;
	if (collisionController->Raycast(rayOrigin, rayDirection, hitInfo, collider))
	{
		if (hitInfo.collider != nullptr && hitInfo.collider->GetGameObject() != nullptr)
		{
			auto enemy = hitInfo.collider->GetGameObject()->GetComponent<Enemy>();
			if (enemy != nullptr)
				enemy->TakeDamage(15.0f);
		}
	}
}

void Player::HandleMovement(float deltaTime)
{
	//Debug::Log("velocity", glm::to_string(velocity));

	auto transform = GetGameObject()->GetTransform();
	glm::vec3 move = glm::vec3(0.0f);

	if (Input::GetKeys()[GLFW_KEY_W])
		move += transform->GetForwardVector() * accelerationSpeed * deltaTime;
	if (Input::GetKeys()[GLFW_KEY_S])
		move -= transform->GetForwardVector() * accelerationSpeed * deltaTime;

	if (Input::GetKeys()[GLFW_KEY_A])
		move -= transform->GetRightVector() * accelerationSpeed * deltaTime;
	if (Input::GetKeys()[GLFW_KEY_D])
		move += transform->GetRightVector() * accelerationSpeed * deltaTime;

	if (Input::GetKeys()[GLFW_KEY_SPACE] && isGrounded)
		move += transform->GetUpVector() * 5.0f;

	auto velocityXZ = glm::vec3{ move.x, 0, move.z };
	auto maxVelocity = glm::normalize(glm::vec3(1, 0, 1)) * moveSpeed;
	velocityXZ = glm::clamp(velocityXZ, -maxVelocity, maxVelocity);

	velocity += velocityXZ;
	velocity.y += move.y;
	velocity += gravity * deltaTime;
}
