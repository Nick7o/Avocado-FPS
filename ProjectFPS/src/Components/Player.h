#pragma once

#include "Puppet.h"

class CollisionController;
class CameraController;
class Light;

class Player : public Puppet
{
public:
	GameObject* weapon;
	Light* weaponFlashlight;

	void Fire();

private:
	void Start() override;
	void Update(float deltaTime) override;

	void HandleMovement(float deltaTime);

	CameraController* cameraController;
};

