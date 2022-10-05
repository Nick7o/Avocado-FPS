#pragma once

#include <SceneManager.h>

class Game
{
public:
	void Initialize();
	void Update(float deltaTime);

	static float GetLastDeltaTime() { return game->lastDeltaTime; }

	//GameObject* Find(const std::string& name);

private:
	void CreateCamera();

private:
	static Game* game;

	float lastDeltaTime;
};