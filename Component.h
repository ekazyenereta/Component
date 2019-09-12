#pragma once
#include "Object.h"

class GameObject;
class Component :
	public Object
{
public:
	Component();
	virtual ~Component();
	GameObject* GetGameObject();
protected:
	GameObject* m_game_object;
};
