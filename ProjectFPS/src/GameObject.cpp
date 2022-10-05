#include "pch.h"
#include "GameObject.h"
#include "Component.h"
#include "Components/Transform.h"

#include "SceneManager.h"
#include "Scene.h"

GameObject::GameObject()
{
	auto t = new Transform();
	transform = AddComponent(t);

	SceneManager::GetInstance().GetCurrentScene()->AddGameObject(this);
	//SceneManager::GetInstance().GetCurrentScene()->AddGameObject(this);
}

GameObject::GameObject(const std::string name) : GameObject()
{
	this->name = name;
}

GameObject::~GameObject()
{
	// delete all components...
}

//template<class T>
Component* GameObject::AddComponent(Component* component)
{
	//static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

	auto componentPointer = std::unique_ptr<Component>(component);
	componentPointer->SetGameObject(*this);
	componentPointer->Start();
	this->components.push_back(std::move(componentPointer));

	return component;
}

void GameObject::Update(float deltaTime)
{
	for (const auto& component : components)
	{
		component->Update(deltaTime);
	}
}
