#pragma once

#include <vector>
#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

class MeshImporter
{
public:
	MeshImporter();

	std::vector<std::shared_ptr<Mesh>>* Load(const std::string path);

    unsigned int GetMaterialCount() { return materialCount; }
private:
    std::string path = "";
    std::vector<std::shared_ptr<Mesh>> meshes;
    unsigned int materialCount = 0;

    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
};

