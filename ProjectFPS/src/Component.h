#pragma once

#include "Object.h"
#include "GameObject.h"
#include "Game.h"

#include <memory>

class Component : public Object
{
public:
	bool enabled = true;
public:
	virtual ~Component() { }

	virtual void Start() {};
	virtual void Update(float deltaTime) {};

	inline GameObject* GetGameObject() const { return gameObject; }

private:
	void SetGameObject(GameObject& gameObject);

	GameObject* gameObject;

	friend Game;
	friend GameObject;
};

