#pragma once
#include "Object.h"

class GameObject;
class Component :
	public Object
{
public:
	friend GameObject;
public:
	Component();
	Component(const std::string& str);
	Component(const std::string& str, GameObject* obj);
	Component(GameObject* obj);
	virtual ~Component();
private:
	void SetGameObject(GameObject* obj) {
		m_GameObject = obj;
	}
protected:
	GameObject* m_GameObject;
};
