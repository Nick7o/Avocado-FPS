#pragma once

#include <memory>
#include "Scene.h"

class SceneManager
{
public:
    void Update(float deltaTime);

    inline Scene* GetCurrentScene()
    {
        if (currentScene == nullptr)
            currentScene = std::make_unique<Scene>();

        return currentScene.get();
    }

    static SceneManager& GetInstance()
    {
        static SceneManager instance;
        return instance;
    }

    SceneManager(SceneManager const&) = delete;
    void operator=(SceneManager const&) = delete;

private:
    SceneManager() {};
    
    std::unique_ptr<Scene> currentScene;
};

