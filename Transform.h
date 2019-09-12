#pragma once
#include "Component.h"

class Transform :
	public Component
{
public:
	Transform();
	Transform(GameObject* _game_object);
	virtual ~Transform();
};
