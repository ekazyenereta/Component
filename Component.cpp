#include "Component.h"
#include "GameObject.h"

Component::Component() :
	m_game_object(nullptr)
{
}

Component::~Component()
{
}

GameObject* Component::GetGameObject()
{
	return m_game_object;
}
