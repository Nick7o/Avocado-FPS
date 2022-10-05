#include "pch.h"

#include "Game.h"
#include "GameObject.h"
#include "Components/CameraController.h"
#include "MeshImporter.h"
#include "Components/MeshRenderer.h"
#include "Levels/LevelDescription.h"

#include "Input.h"
#include "Rendering/Renderer.h"

Game* Game::game;

void Game::Initialize()
{
	this->game = this;

	CreateCamera();

	auto levelDescriptor = LevelDescription();
	levelDescriptor.Load();

	Input::KeyChange += [](int key, bool isDown) {
		if (key == GLFW_KEY_F10 && isDown)
			Renderer::AllowWireframeDraws = !Renderer::AllowWireframeDraws;
	};
}

void Game::Update(float deltaTime)
{
	SceneManager::GetInstance().Update(deltaTime);

	lastDeltaTime = deltaTime;
}

void Game::CreateCamera()
{
	auto cameraGO = new GameObject();
	auto cameraController = cameraGO->AddComponent(new CameraController());

	cameraController->camera.SetProjection(Camera::PerspectiveProjectionData(60.0f, 16/9.0f, 0.001f, 100.0f));
}