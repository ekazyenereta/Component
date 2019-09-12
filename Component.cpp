#include "Component.h"
#include "GameObject.h"

Component::Component() :
	Object("Component"), m_game_object(nullptr)
{
}

Component::Component(const std::string& str) :
	Object(str), m_game_object(nullptr)
{
}

Component::Component(const std::string& str, GameObject* _game_object) :
	Object(str), m_game_object(_game_object)
{
}

Component::Component(GameObject* _game_object) :
	Object("Component"), m_game_object(_game_object)
{
}

Component::~Component()
{
}
