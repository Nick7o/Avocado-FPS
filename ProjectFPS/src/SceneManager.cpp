#include "pch.h"
#include "SceneManager.h"

void SceneManager::Update(float deltaTime)
{
	GetCurrentScene()->Update(deltaTime);
}
