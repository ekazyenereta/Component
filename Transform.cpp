#include "Transform.h"
#include "GameObject.h"

Transform::Transform() :
	Component("Transform")
{
}

Transform::Transform(GameObject* _game_object) :
	Component("Transform", _game_object)
{
}

Transform::~Transform()
{
}
