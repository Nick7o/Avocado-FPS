#pragma once
#include <Mesh.h>

// Cube
class Shapes
{
public:
	static std::shared_ptr<Mesh> CreateCube(const glm::vec3& size, bool reverseOrder = true);
};