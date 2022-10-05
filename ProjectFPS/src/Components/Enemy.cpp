#include "pch.h"
#include "Enemy.h"

#include "Transform.h"
#include "CollisionController.h"
#include "Colliders.h"

void Enemy::Start()
{
	Puppet::Start();

	collider = GetGameObject()->AddComponent(new BoxCollider());
	collider->SetSize(glm::vec3(1.0f, height, 1.0f));
	collisionController->colliders.push_back(collider);
}

void Enemy::Update(float deltaTime)
{
	auto t = GetGameObject()->GetTransform();
	if (t->position.y > 200.0f)
		return;

	t->rotation += glm::vec3{0.00f, 0.01f, 0.0f};

	collider->SetPosition(t->position + glm::vec3(0, height * 0.5f, 0));
	collider->SetRotation(t->rotation);

	if (!IsDead())
		velocity += gravity * deltaTime;
	else
	{
		velocity -= gravity * deltaTime;
		if (t->scale.y > 0.1f)
			t->scale.y -= 2.0f * deltaTime;
	}

	HandlePhysics(deltaTime);

	if (decisionTimer < decisionInterval)
	{
		velocity += lastDecision * deltaTime * 10.0f;
		decisionTimer += deltaTime;
		return;
	}

	decisionTimer = 0;

	glm::vec2 move = glm::normalize(glm::vec2((float)std::rand() / RAND_MAX, (float)std::rand() / RAND_MAX) * 2.0f - 1.0f);

	auto velocityXZ = glm::vec3{ move.x, 0, move.y };
	auto maxVelocity = glm::normalize(glm::vec3(1, 0, 1)) * moveSpeed;
	velocityXZ = glm::clamp(velocityXZ, -maxVelocity, maxVelocity);

	velocity += velocityXZ;

	lastDecision = velocityXZ;
}
