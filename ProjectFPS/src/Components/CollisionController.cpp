#include "pch.h"
#include "CollisionController.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "CameraController.h"
#include "Shapes.h"
#include "Rendering/Renderer.h"

bool CollisionController::Raycast(glm::vec3 rayOrigin, glm::vec3 rayDirection, HitInfo& hitInfo, Collider* ignore)
{
	bool foundHit = false;

	hitInfo.intersectionPoint = glm::vec3(1.0f) * 95100000.0f;

	for (auto collider : colliders)
	{
		if (collider == ignore)
			continue;

		glm::vec3 point;
		if (!collider->Intersect(rayOrigin, rayDirection, point))
			continue;

		foundHit = true;

		if (glm::distance2(rayOrigin, point) < glm::distance2(rayOrigin, hitInfo.intersectionPoint))
		{
			hitInfo.collider = collider;
			hitInfo.intersectionPoint = point;
		}
	}

	return foundHit;
}

void CollisionController::Start()
{
	unitCube = Shapes::CreateCube(glm::vec3(1.0f));

	auto boxCollider = GetGameObject()->AddComponent(new BoxCollider());
	boxCollider->SetPosition({0.0f, -0.48f, 0.0f});
	boxCollider->SetSize({ 200.0f, 1.0f, 200.0f });
	colliders.push_back(boxCollider);
}

void CollisionController::Update(float deltaTime)
{
	if (!drawDebug)
		return;

	for (auto collider : colliders)
	{
		if (typeid(*collider) == typeid(BoxCollider))
		{
			auto& boxCollider = dynamic_cast<BoxCollider&>(*collider);
			Renderer::DrawMeshWireframe(unitCube, nullptr, boxCollider.GetMatrix(), CameraController::GetCurrent()->camera);
		}
	}
}
