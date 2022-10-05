#pragma once
#include "Component.h"

#include "Colliders.h"
#include "Mesh.h"

struct HitInfo
{
	glm::vec3 intersectionPoint;
	Collider* collider;
};

class CollisionController : public Component
{
public:
	bool Raycast(glm::vec3 rayOrigin, glm::vec3 rayDirection, HitInfo& hitInfo, Collider* ignore = nullptr);

	std::list<Collider*> colliders;

private:
	void Start() override;
	void Update(float deltaTime) override;

	bool drawDebug = true;
	std::shared_ptr<Mesh> unitCube;
};

