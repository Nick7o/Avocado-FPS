#include "pch.h"
#include "MeshImporter.h"

MeshImporter::MeshImporter()
{

}

std::vector<std::shared_ptr<Mesh>>* MeshImporter::Load(const std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        Debug::LogError("Assimp", importer.GetErrorString());
        return nullptr;
    }

    this->path = path.substr(0, path.find_last_of('/'));

    materialCount = scene->mNumMaterials;
    ProcessNode(scene->mRootNode, scene);

    auto result = new std::vector<std::shared_ptr<Mesh>>(meshes);
	return result;
}

void MeshImporter::ProcessNode(aiNode* node, const aiScene* scene)
{
    // Processing node meshes
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        auto mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(std::move(ProcessMesh(mesh, scene)));
    }

    // Processing node's children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        ProcessNode(node->mChildren[i], scene);
}

std::shared_ptr<Mesh> MeshImporter::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec3> uv0;
    std::vector<glm::vec4> colors0;

    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        positions.push_back(position);

        if (mesh->HasNormals())
        {
            glm::vec3 normal;
            normal.x = mesh->mNormals[i].x;
            normal.y = mesh->mNormals[i].y;
            normal.z = mesh->mNormals[i].z;
            normals.push_back(normal);
        }
        else
            normals.push_back(glm::vec3());

        if (mesh->HasTangentsAndBitangents())
        {
            glm::vec3 tangent;
            tangent.x = mesh->mTangents[i].x;
            tangent.y = mesh->mTangents[i].y;
            tangent.z = mesh->mTangents[i].z;
            tangents.push_back(tangent);
        }

        if (mesh->HasTextureCoords(0))
        {
            glm::vec3 uv0Coord;
            uv0Coord.x = mesh->mTextureCoords[0][i].x;
            uv0Coord.y = mesh->mTextureCoords[0][i].y;
            uv0Coord.z = mesh->mTextureCoords[0][i].z;
            uv0.push_back(uv0Coord);
        }
        else
            uv0.push_back(glm::vec3());

        if (mesh->HasVertexColors(0))
        {
            glm::vec4 col0;
            col0.r = mesh->mColors[0][i].r;
            col0.g = mesh->mColors[0][i].g;
            col0.b = mesh->mColors[0][i].b;
            col0.a = mesh->mColors[0][i].a;
            colors0.push_back(col0);
        }
    }
    
    // Loading indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    auto vertexCount = positions.size();
    auto gameMesh = new Mesh(positions, indices);
    //gameMesh->SetVertices(positions);
    //gameMesh->SetIndices(indices);

    if (normals.size() == vertexCount)
        gameMesh->SetNormals(normals);
    if (uv0.size() == vertexCount)
        gameMesh->SetUV0(uv0);
    if (colors0.size() == vertexCount)
        gameMesh->SetColors(colors0);
    if (tangents.size() == vertexCount)
        gameMesh->SetTangents(tangents);

    gameMesh->SendData();

    return std::shared_ptr<Mesh>(gameMesh);
}
