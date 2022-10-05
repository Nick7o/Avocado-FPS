#pragma once

#include "pch.h"

#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLWindow.h"
#include "Debug.h"
#include "Input.h"
#include "Mesh.h"
#include "MeshImporter.h"
#include "GameObject.h"
#include "Rendering/Renderer.h"

#include "Components/Transform.h"
#include "Components/CameraController.h"

#include "Rendering/Shader.h"

static OpenGLWindow* mainWindow;

void LoadShaders()
{
	// Basic shader
	std::string baseShaderPath = "resources/Shaders/shader";
	auto baseShader = new Shader("Base");
	baseShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Missing shader
	baseShaderPath = "resources/Shaders/Missing";
	auto missingShader = new Shader("__ERROR_SHADER");
	missingShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Shadow map (depth) shader
	baseShaderPath = "resources/Shaders/Shadow";
	auto shadowShader = new Shader("__ShadowMap");
	shadowShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// UI shader
	baseShaderPath = "resources/Shaders/UI";
	auto uiShader = new Shader("UI");
	uiShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Skybox shader
	baseShaderPath = "resources/Shaders/Skybox";
	auto skyboxShader = new Shader("Skybox");
	skyboxShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");

	// Standard shader
	baseShaderPath = "resources/Shaders/Standard";
	auto standardShader = new Shader("Standard");
	standardShader->Load(baseShaderPath + ".vert", baseShaderPath + ".frag");
}

void Initialize()
{
	std::srand(std::time(nullptr));

	glfwSetInputMode(mainWindow->GetWindowPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	LoadShaders();
	Renderer::Init();
}

#include "glm/gtx/string_cast.hpp"

int main()
{
	mainWindow = new OpenGLWindow(1600, 900);
	mainWindow->Initialize();
	Renderer::SetWindowResolutionInfo(1600, 900);

	Initialize();

	Game game;
	game.Initialize();

	double lastTime = glfwGetTime();

	while (!mainWindow->ShouldClose())
	{
		Renderer::Render();
		Renderer::Flush();

		auto currentTime = glfwGetTime();
		auto deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();

		if (Input::GetKeys()[GLFW_KEY_ESCAPE])
			glfwSetWindowShouldClose(mainWindow->GetWindowPointer(), true);

		game.Update(deltaTime);

		mainWindow->SwapBuffers();
	}

	delete mainWindow;
	return 0;
}