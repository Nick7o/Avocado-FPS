#pragma once

#include "glm/glm.hpp"

#include <Rendering/Framebuffer.h>
#include <Rendering/TextureCube.h>

#include "Camera.h"
#include "Material.h"
#include "Components/Light.h"

class Mesh;
class Light;

constexpr unsigned int MaxLightCount = 8;

class Renderer
{
public:
	struct DrawCommand
	{
		std::shared_ptr<Mesh> mesh;
		std::shared_ptr<Material> material;
		glm::mat4 model;
		Camera& camera;
		bool castShadows;
		bool receiveShadows;
	};

	static bool AllowWireframeDraws;

	static void Init();

	static void Render();
	static void Flush();

	static void ShadowPass();
	static void BasePass();
	static void PostFXPass();

	//static void DrawMesh(const Mesh* mesh, Material* material, const glm::mat4& localToWorld, Camera& camera);
	static void DrawMeshWireframe(const std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const glm::mat4& localToWorld, Camera& camera);
	static void DrawMesh(const std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const glm::mat4& localToWorld, Camera& camera, bool castShadows = true, bool receiveShadows = true);
	static void ExecuteDrawCommands(std::list<std::unique_ptr<DrawCommand>>& drawCommands);
	static void ExecuteDrawCommand(DrawCommand& command);

	static void SetSkybox(std::shared_ptr<TextureCube> skyboxTexture);

	static void SetFramebuffer(Framebuffer* framebuffer);
	static void SetWindowResolutionInfo(unsigned int width, unsigned int height)
	{
		windowWidth = width;
		windowHeight = height;
		glViewport(0, 0, width, height);
	}

	static unsigned int GetWindowWidth() { return windowWidth; }
	static unsigned int GetWindowHeight() { return windowHeight; }

private:
	static bool initialized;
	static unsigned int windowWidth, windowHeight;

	static std::shared_ptr<Mesh> skyboxMesh;
	static std::shared_ptr<TextureCube> skyboxTexture;

	static std::list<std::unique_ptr<DrawCommand>> drawCommands;
	static std::vector<Light*> lights;
	static std::unordered_map<Light*, std::unique_ptr<Framebuffer>> shadowMaps;

	static void SetLightData(Shader& shader, int lightNumber, Light& light);
};

