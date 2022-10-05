#include "pch.h"
#include "Shapes.h"

std::shared_ptr<Mesh> Shapes::CreateCube(const glm::vec3& size, bool reverseOrder)
{
	glm::vec3 halfSize = size * 0.5f;

	std::vector<glm::vec3> vertices {
		{ -halfSize.x, -halfSize.y, -halfSize.z}, { halfSize.x, -halfSize.y, -halfSize.z },
		{ halfSize.x, halfSize.y, -halfSize.z }, { -halfSize.x, halfSize.y, -halfSize.z	},
		{ -halfSize.x, -halfSize.y, halfSize.z }, { halfSize.x, -halfSize.y, halfSize.z },
		{ halfSize.x, halfSize.y, halfSize.z }, { -halfSize.x, halfSize.y, halfSize.z }
	};

	std::vector<unsigned int> indices {
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};

	if (reverseOrder)
		std::reverse(indices.begin(), indices.end());

	return std::make_shared<Mesh>(vertices, indices);
}