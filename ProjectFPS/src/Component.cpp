#include "pch.h"
#include "Component.h"

void Component::SetGameObject(GameObject& gameObject)
{
	this->gameObject = &gameObject;
}