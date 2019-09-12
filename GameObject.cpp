#include "GameObject.h"
#include "Transform.h"

GameObject::GameObject() :
	m_transform(new Transform)
{
}

GameObject::~GameObject()
{
	delete m_transform;
	for (auto& itr : m_component_list)
		if (itr != nullptr)
			Destroy(itr);
}


template <>
Transform* GameObject::GetComponent()
{
	return m_transform;
}
