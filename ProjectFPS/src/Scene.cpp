#include "pch.h"
#include "Scene.h"

GameObject* Scene::Find(const std::string& name)
{
    auto it = std::find_if(gameObjects.begin(), gameObjects.end(), [name](std::unique_ptr<GameObject>& go) {
            return go->name.find(name) != std::string::npos;
        });

    return it == gameObjects.end() ? nullptr : (*it).get();
}

void Scene::AddGameObject(GameObject* gameObject)
{
    if (gameObject == nullptr)
        return;

    if (std::find_if(gameObjects.begin(), gameObjects.end(), 
        [gameObject](std::unique_ptr<GameObject>& go) {
            return go.get() == gameObject; 
        }) != gameObjects.end())
    {
        Debug::LogError("Scene", "Scene already have registered this game object!");
        return;
    }

    gameObjects.push_back(std::unique_ptr<GameObject>(gameObject));
}

void Scene::Update(float deltaTime)
{
    for (const auto& go : gameObjects)
    {
        go->Update(deltaTime);
    }
}
