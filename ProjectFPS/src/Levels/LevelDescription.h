#pragma once

#include "Mesh.h"
#include "Rendering/Texture.h"
#include "Rendering/Material.h"

class MeshRenderer;
class CollisionController;
class Enemy;

class LevelDescription
{
public:
	virtual void Load();

	void LoadLevel();
	void CreateLights();
	void CreatePlayer();
	Enemy* CreateEnemy(glm::vec3 pos);
	void CreateCollision(CollisionController& collisionController);

	MeshRenderer* LoadAndCreate(const std::string& path);
};

