#pragma once
#include "Component.h"

class Transform :
	public Component
{
public:
	Transform();
	Transform(GameObject* obj);
	~Transform();
};
