#pragma once

#include "Puppet.h"

class BoxCollider;

class Enemy : public Puppet
{
public:
	float decisionInterval = 0.5f;

private:
	void Start() override;
	void Update(float deltaTime) override;

	float decisionTimer = 0.0f;
	glm::vec3 lastDecision = glm::vec3(0.0f, 0.0f, 0.0f);
};