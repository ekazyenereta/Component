#include "GameObject.h"
#include "Transform.h"

GameObject::GameObject() :
	Object("GameObject")
{
	m_transform = AddComponent(new Transform(this));
}

GameObject::GameObject(const std::string& _str) :
	Object(_str)
{
	m_transform = AddComponent(new Transform(this));
}

GameObject::~GameObject()
{
	m_component_list.clear();
}

template <>
reference::IReference<Transform> GameObject::GetComponent()
{
	return m_transform;
}
