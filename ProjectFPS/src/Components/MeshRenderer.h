#pragma once

#include "Component.h"
#include "Mesh.h"

class MeshRenderer : public Component
{
public:
	void Update(float deltaTime) override;

	void Draw();

	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<Material>> materials;

	bool castShadows = true;
};

