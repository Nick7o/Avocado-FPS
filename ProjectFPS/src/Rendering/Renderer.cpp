#include "pch.h"
#include "Renderer.h"

#include "Components/CameraController.h"
#include "Scene.h"
#include "Mesh.h"
#include "Components/Transform.h"
#include <OpenGLWindow.h>
#include <Rendering/TextureFormat.h>

#include "Shapes.h"

#include "glm/gtx/string_cast.hpp"

std::list<std::unique_ptr<Renderer::DrawCommand>> Renderer::drawCommands;
std::vector<Light*> Renderer::lights;
std::unordered_map<Light*, std::unique_ptr<Framebuffer>> Renderer::shadowMaps;

unsigned int Renderer::windowWidth;
unsigned int Renderer::windowHeight;

bool Renderer::AllowWireframeDraws = false;
bool Renderer::initialized;

std::shared_ptr<Mesh> Renderer::skyboxMesh;
std::shared_ptr<TextureCube> Renderer::skyboxTexture;

void Renderer::Init()
{
	if (initialized)
		return;

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	skyboxMesh = Shapes::CreateCube(glm::vec3(1.0f), false);

	initialized = true;
}

void Renderer::Render()
{
	SceneManager::GetInstance().GetCurrentScene()->GetComponents(lights);

	ShadowPass();
	BasePass();
	//PostFXPass();
}

void Renderer::Flush()
{
	drawCommands.clear();
	lights.clear();
}

void Renderer::ShadowPass()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

	static Shader* shadowMapShader = Shader::Find("__ShadowMap");
	static std::shared_ptr<Material> shadowMaterial = std::make_shared<Material>(shadowMapShader);

	for (size_t i = 0; i < lights.size(); i++)
	{
		if (lights[i]->type != LightType::Directional)
			continue;

		// Finding or creating FBO
		Framebuffer* shadowFBO;
		if (shadowMaps.count(lights[i]) == 0)
		{
			auto fb = std::make_unique<Framebuffer>(4096, 4096, TextureFormat::D32, false);
			shadowMaps[lights[i]] = std::move(fb);
		}

		shadowFBO = shadowMaps[lights[i]].get();

		// Setting up
		SetFramebuffer(shadowFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);

		GLenum fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			Debug::LogError("Renderer::ShadowPass", "Framebuffer error: " + std::to_string(fboStatus));
			return;
		}

		// Rendering scene
		Camera lightView = *lights[i]->GetCamera().get();

		for (auto& cmd : drawCommands)
		{
			if (!cmd->castShadows)
				continue;

			DrawCommand shadowCmd = { cmd->mesh, shadowMaterial, cmd->model, lightView, cmd->castShadows };
			ExecuteDrawCommand(shadowCmd);
		}
	}
}

void Renderer::BasePass()
{
	SetFramebuffer(nullptr);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Skybox
	static auto skyboxShader = Shader::Find("Skybox");
	static auto skyboxMaterial = std::make_shared<Material>(skyboxShader);
	skyboxMaterial->SetTexture("skybox", skyboxTexture);

	glDepthMask(GL_FALSE);

	Camera camera;
	auto cameraController = CameraController::GetCurrent();
	if (cameraController == nullptr)
	{
		camera = Camera();
		camera.position = glm::vec3(0.0f);
		camera.rotation = glm::vec3(0.0f);
		camera.SetProjection(Camera::PerspectiveProjectionData());
	}
	else
		camera = cameraController->camera;

	auto drawSkyboxCmd = DrawCommand{skyboxMesh, skyboxMaterial, glm::mat4(0.0f), camera, false};
	ExecuteDrawCommand(drawSkyboxCmd);

	glDepthMask(GL_TRUE);

	// Opaque geometry
	glEnable(GL_FRAMEBUFFER_SRGB);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	ExecuteDrawCommands(drawCommands);
}

void Renderer::PostFXPass()
{
	
}

void Renderer::DrawMeshWireframe(const std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, const glm::mat4& localToWorld, Camera& camera)
{
	if (!AllowWireframeDraws)
		return;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	auto cmd = DrawCommand{ mesh, material, localToWorld, camera };
	ExecuteDrawCommand(cmd);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Renderer::DrawMesh(const std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material,
	const glm::mat4& localToWorld, Camera& camera, bool castShadows, bool receiveShadows)
{
	if (mesh == nullptr)
		return;

	drawCommands.push_back(std::unique_ptr<DrawCommand>(new DrawCommand{ mesh, material, localToWorld, camera, castShadows, receiveShadows }));
}

void Renderer::ExecuteDrawCommands(std::list<std::unique_ptr<DrawCommand>>& drawCommands)
{
	if (drawCommands.empty())
		return;

	for (auto& drawCmd : drawCommands)
		ExecuteDrawCommand(*drawCmd);
}

void Renderer::ExecuteDrawCommand(DrawCommand& command)
{
	if (command.mesh == nullptr)
		return;

	static auto missingMaterial = new Material(Shader::GetErrorShader());

	auto material = command.material.get();

	if (command.material == nullptr || command.material->shader == nullptr)
		material = missingMaterial;

	auto shader = material->shader;

	// Properties provided by the renderer
	shader->SetVec3("uv0Tilling", glm::vec3(1.0f)); // default tilling

	shader->SetMatrix4("model", command.model);
	shader->SetMatrix4("view", command.camera.GetViewMatrix());
	shader->SetMatrix4("projection", command.camera.GetProjectionMatrix());
	shader->SetVec3("viewPos", command.camera.position);

	material->Bind();
	int nextFreeTextureId = material->GetTextureCount();

	// Light
	std::sort(lights.begin(), lights.end(), [command](Light* a, Light* b) {
		auto aPos = a->GetGameObject()->GetTransform()->Position;
		auto bPos = b->GetGameObject()->GetTransform()->Position;
		return glm::distance2(aPos, command.camera.position) < glm::distance2(bPos, command.camera.position);
		});

	int lightNumber = 0;
	for (auto light : lights)
	{
		if (lightNumber >= MaxLightCount && light->type != LightType::Directional)
			continue;

		SetLightData(*shader, lightNumber, *light);

		if (command.receiveShadows)
		{
			// Binding a shadow map
			if (shadowMaps.count(light) > 0 && light->type == LightType::Directional)
			{
				glActiveTexture(GL_TEXTURE0 + nextFreeTextureId);
				shadowMaps[light]->depthAttachment->Bind();
				shader->SetInt("directionalLightShadowMap", nextFreeTextureId);
				nextFreeTextureId++;
			}
		}

		if (light->type != LightType::Directional)
			lightNumber++;
	}
	shader->SetInt("lightCount", lightNumber);

	if (skyboxTexture != nullptr)
	{
		glActiveTexture(GL_TEXTURE0 + nextFreeTextureId);
		skyboxTexture->Bind();
		shader->SetInt("skybox", nextFreeTextureId);
		nextFreeTextureId++;
	}

	glBindVertexArray(command.mesh->VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(command.mesh->indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	material->Unbind();
}

void Renderer::SetSkybox(std::shared_ptr<TextureCube> skyboxTexture)
{
	Renderer::skyboxTexture = skyboxTexture;
}

void Renderer::SetFramebuffer(Framebuffer* framebuffer)
{
	if (framebuffer == nullptr)
	{
		glViewport(0, 0, windowWidth, windowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return;
	}

	glViewport(0, 0, framebuffer->width, framebuffer->height);
	framebuffer->Bind();
}

/* Sets light data in a shader. LightNumber is ignored for directional lights. */
void Renderer::SetLightData(Shader& shader, int lightNumber, Light& light)
{
	std::string basePath = light.type == LightType::Directional
		? "directionalLight."
		: "lights[" + std::to_string(lightNumber) + "].";

	auto transform = light.GetGameObject()->GetTransform();

	if (light.type != LightType::Directional)
	{
		shader.SetVec3(basePath + "position", transform->Position);
		shader.SetVec3(basePath + "attenuationCLQ", light.attenuation.ToVec3());
		shader.SetFloat(basePath + "spotAngle", light.type == LightType::Spot ? cosf(light.spotAngle) : -1.0f);
		shader.SetFloat(basePath + "outerSpotAngle", light.type == LightType::Spot ? cosf(light.outerSpotAngle) : -1.0f);

		basePath += "base.";
	}

	shader.SetVec3(basePath + "direction", transform->GetForwardVector());
	shader.SetVec3(basePath + "color", light.color);
	shader.SetFloat(basePath + "intensity", light.intensity);

	if (shadowMaps.count(&light) > 0)
	{
		auto camera = light.GetCamera();
		auto lightSpaceMatrix = camera->GetProjectionMatrix() * camera->GetViewMatrix();
		shader.SetMatrix4(basePath + "lightSpaceMatrix", lightSpaceMatrix);
	}
}
