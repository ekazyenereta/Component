#include "Component.h"
#include "GameObject.h"

Component::Component() :
	Object("Component"), m_GameObject(nullptr)
{
}

Component::Component(const std::string& str) :
	Object(str), m_GameObject(nullptr)
{
}

Component::Component(const std::string& str, GameObject* obj) :
	Object(str), m_GameObject(obj)
{
}

Component::Component(GameObject* obj) :
	Object("Component"), m_GameObject(obj)
{
}

Component::~Component()
{
}
