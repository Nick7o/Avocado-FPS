#pragma once

#include "Object.h"

#include <vector>
#include <memory>

class Component;
class Transform;

class GameObject : public Object
{
public:
	GameObject();
	GameObject(const std::string name);
	~GameObject();

	template<class T>
	T* AddComponent(T* component);
	Component* AddComponent(Component* component);

	template<class T>
	inline T* GetComponent() const;

	template<class T>
	inline void GetComponents(std::vector<T*>& componentContainer) const;

	Transform* GetTransform() { return transform; }

private:
	void Update(float deltaTime);

private:
	friend class Scene;

	Transform* transform;
	std::vector<std::unique_ptr<Component>> components;
};

template<class T>
inline T* GameObject::AddComponent(T* component)
{
	static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

	return static_cast<T*>(AddComponent(static_cast<Component*>(component)));
}

template<class T>
inline T* GameObject::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

	T* result = nullptr;
	for (auto const& c : components)
	{
		result = dynamic_cast<T*>(c.get());
		if (result != nullptr)
			break;
	}

	return result;
}

template<class T>
inline void GameObject::GetComponents(std::vector<T*>& componentContainer) const
{
	static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");

	for (auto const& c : components)
	{
		T* component = dynamic_cast<T*>(c.get());
		if (component != nullptr)
		{
			componentContainer.push_back(component);
		}
	}
}
