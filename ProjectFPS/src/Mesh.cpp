#include "pch.h"
#include "Mesh.h"
#include "Rendering/Material.h"
#include "Debug.h"

Mesh::Mesh(std::vector<glm::vec3> vertices, std::vector<uint32_t> indices)
{
    this->vertices = vertices;
    this->indices = indices;

    SetupMesh();
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Mesh::SetVertices(std::vector<glm::vec3> vertices)
{
    this->vertices = vertices;
}

void Mesh::SetUV0(std::vector<glm::vec3> uv)
{
    this->uv0 = uv;
}

void Mesh::SetNormals(std::vector<glm::vec3> normals)
{
    this->normals = normals;
}

void Mesh::SetTangents(std::vector<glm::vec3> tangents)
{
    this->tangents = tangents;
}

void Mesh::SetIndices(std::vector<uint32_t> indices)
{
    this->indices = indices;
}

void Mesh::SetColors(std::vector<glm::vec4> colors)
{
    this->colors = colors;
}

void Mesh::SetupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    SendData();
}

void Mesh::SendData()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, GetBufferSize(), 0, GL_STATIC_DRAW);

    // Buffer data
    if (!vertices.empty())
        glBufferSubData(GL_ARRAY_BUFFER, GetVertexOffset(), vertices.size() * sizeof(vertices[0]), &vertices[0]);
    if (!colors.empty())
        glBufferSubData(GL_ARRAY_BUFFER, GetColorsOffset(), vertices.size() * sizeof(colors[0]), &colors[0]);
    if (!normals.empty())
        glBufferSubData(GL_ARRAY_BUFFER, GetNormalsOffset(), vertices.size() * sizeof(normals[0]), &normals[0]);
    if (!tangents.empty())
        glBufferSubData(GL_ARRAY_BUFFER, GetTangentsOffset(), vertices.size() * sizeof(tangents[0]), &tangents[0]);
    if (!uv0.empty())
        glBufferSubData(GL_ARRAY_BUFFER, GetUV0Offset(), vertices.size() * sizeof(uv0[0]), &uv0[0]);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

    if (!vertices.empty())
    {
        glEnableVertexAttribArray(VertexAttributeLocation);
        glVertexAttribPointer(VertexAttributeLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)GetVertexOffset());
    }
    if (!normals.empty())
    {
        glEnableVertexAttribArray(NormalAttributeLocation);
        glVertexAttribPointer(NormalAttributeLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)GetNormalsOffset());
    }
    if (!tangents.empty())
    {
        glEnableVertexAttribArray(TangentAttributeLocation);
        glVertexAttribPointer(TangentAttributeLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)GetTangentsOffset());
    }
    if (!colors.empty())
    {
        glEnableVertexAttribArray(ColorAttributeLocation);
        glVertexAttribPointer(ColorAttributeLocation, 4, GL_FLOAT, GL_FALSE, 0, (void*)GetColorsOffset());
    }
    if (!uv0.empty())
    {
        glEnableVertexAttribArray(UV0AttributeLocation);
        glVertexAttribPointer(UV0AttributeLocation, 3, GL_FLOAT, GL_FALSE, 0, (void*)GetUV0Offset());
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

int Mesh::GetBufferSize()
{
    int size = 0;

    size += GetSize(vertices);
    size += GetSize(colors);
    size += GetSize(normals);
    size += GetSize(tangents);
    size += GetSize(uv0);

    return size;
}

int Mesh::GetVertexOffset()
{
    return 0;
}

int Mesh::GetColorsOffset()
{
    return GetSize(vertices);
}

int Mesh::GetNormalsOffset()
{
    return GetColorsOffset() + GetSize(colors);
}

int Mesh::GetTangentsOffset()
{
    return GetNormalsOffset() + GetSize(normals);
}

int Mesh::GetUV0Offset()
{
    return GetTangentsOffset() + GetSize(tangents);
}