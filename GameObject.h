#pragma once
#include <cstdio>
#include <iostream>
#include <list>
#include "Component.h"

class Transform;
class GameObject :
	public Object
{
private:
	class _InputComponent :
		public Component {
	public:
		_InputComponent() {}
		~_InputComponent() {}
		void SetGameObject(GameObject* _Ptr) {
			m_game_object = _Ptr;
		}
	};
public:
	GameObject();
	virtual ~GameObject();

	template <class _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	_Ty* AddComponent(_Ty* _Ptr)
	{
		static_assert(isExtended, "AddComponent<> : T is not inherited from Component Class");
		((_InputComponent*)_Ptr)->SetGameObject(this);
		m_component_list.push_back(_Ptr);
		return _Ptr;
	}

	template <class _Ty>
	_Ty* GetComponent()
	{
		for (auto& itr : m_component_list)
		{
			auto ptr = dynamic_cast<_Ty*>(itr);
			if (ptr != nullptr)
				return ptr;
		}
		return nullptr;
	}
private:
	std::list<Component*> m_component_list;
	Transform* m_transform;
};

template <>
Transform* GameObject::GetComponent();
