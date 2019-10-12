#include "Transform.h"
#include "GameObject.h"

Transform::Transform() :
	Component("Transform")
{
}

Transform::Transform(GameObject* obj) :
	Component("Transform", obj)
{
}

Transform::~Transform()
{
}
