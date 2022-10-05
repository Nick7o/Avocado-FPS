#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Rendering/Material.h"

class Mesh
{
public:
	Mesh(std::vector<glm::vec3> vertices, std::vector<uint32_t> indices);
	~Mesh();

	void SetVertices(std::vector<glm::vec3> vertices);
	void SetIndices(std::vector<uint32_t> indices);
	void SetColors(std::vector<glm::vec4> colors);
	void SetNormals(std::vector<glm::vec3> normals);
	void SetTangents(std::vector<glm::vec3> tangents);
	void SetUV0(std::vector<glm::vec3> uv);

	void SendData();

	std::vector<glm::vec3> GetVertices() const { return vertices; }
	std::vector<uint32_t> GetIndices() const { return indices; }
	std::vector<glm::vec4> GetColors() const { return colors; }
	std::vector<glm::vec3> GetNormals() const { return normals; }
	std::vector<glm::vec3> GetTangents() const { return tangents; }
	std::vector<glm::vec3> GetUV0() const { return uv0; }

protected:
	friend class Renderer;
	unsigned int VAO, VBO, EBO;

	std::vector<glm::vec3> vertices;
	std::vector<uint32_t> indices;
	std::vector<glm::vec4> colors;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> uv0;

	void SetupMesh();

	int GetBufferSize();
	int GetVertexOffset();
	int GetColorsOffset();
	int GetNormalsOffset();
	int GetTangentsOffset();
	int GetUV0Offset();

	template <typename T>
	inline int GetSize(const std::vector<T>& vector)
	{
		return vector.size() * sizeof(T);
	}
};