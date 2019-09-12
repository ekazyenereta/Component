#pragma once
#include "Object.h"
#include "Reference.h"

class GameObject;
class Component :
	public Object
{
public:
	Component();
	Component(const std::string& str);
	Component(const std::string& str, GameObject* _game_object);
	Component(GameObject* _game_object);
	virtual ~Component();
protected:
	GameObject* m_game_object;
};
