#pragma once

#include "GameObject.h"

class Scene
{
public:
	GameObject* Find(const std::string& name);

	template<typename T>
	void GetComponents(std::vector<T*>& componentsContainer);

	template<typename T>
	T* GetComponent();

private:
	void AddGameObject(GameObject* gameObject);
	void Update(float deltaTime);

private:
	friend class SceneManager;
	friend class GameObject;

	std::list<std::unique_ptr<GameObject>> gameObjects;
};

template<typename T>
inline void Scene::GetComponents(std::vector<T*>& componentsContainer)
{
	for (auto& go : gameObjects)
	{
		go->GetComponents(componentsContainer);
	}
}

template<typename T>
inline T* Scene::GetComponent()
{
	for (auto& go : gameObjects)
	{
		T* c = go->GetComponent<T>();
		if (c != nullptr)
			return c;
	}

	return nullptr;
}
