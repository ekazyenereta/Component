#include "GameObject.h"
#include "Transform.h"

GameObject::GameObject() :
	Object("GameObject")
{
	m_Transform = AddComponent(new Transform(this));
}

GameObject::GameObject(const std::string& str) :
	Object(str)
{
	m_Transform = AddComponent(new Transform(this));
}

GameObject::~GameObject()
{
	for (auto& itr1 : m_ComponentList) {
		for (auto itr2 : itr1.second)
			if (itr2 != nullptr) {
				delete itr2;
				itr2 = nullptr;
			}
		itr1.second.clear();
	}

	m_ComponentList.clear();
}
 
SharePtr<GameObject> GameObject::AddGameObject()
{
	auto shareObj = SharePtr<GameObject>(new GameObject("GameObject"));
	m_GameObjectChild.emplace_back(shareObj);
	return shareObj;
}

SharePtr<GameObject> GameObject::AddGameObject(const std::string& str)
{
	auto shareObj = SharePtr<GameObject>(new GameObject(str));
	m_GameObjectChild.emplace_back(shareObj);
	return shareObj;
}

SharePtr<GameObject> GameObject::AddGameObject(GameObject* obj)
{
	auto shareObj = SharePtr<GameObject>(obj);
	m_GameObjectChild.emplace_back(shareObj);
	return shareObj;
}

const std::list<SharePtr<GameObject>>& GameObject::GetChild()
{
	return m_GameObjectChild;
}

size_t GameObject::GetNumChild()
{
	return m_GameObjectChild.size();
}

template <>
Transform* GameObject::GetComponent()
{
	return m_Transform;
}
