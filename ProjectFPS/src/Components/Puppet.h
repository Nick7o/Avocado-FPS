#pragma once
#include "Component.h"

#include "glm/glm.hpp"

class CollisionController;
class BoxCollider;

class Puppet : public Component
{
public:
	virtual void HandlePhysics(float deltaTime);
	virtual void TakeDamage(float damage);

	bool IsDead() { return health <= 0.0f; }

	float health = 100.0f;

	CollisionController* collisionController;
	BoxCollider* collider;

	float height = 1.75f;
	float radius = 0.5f;
	float moveSpeed = 5.0f;
	float accelerationSpeed = 25.0f;
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);

	int collisionRays = 16;

protected:
	void Start() override;

	glm::vec3 posChange;
	bool isGrounded;
};

