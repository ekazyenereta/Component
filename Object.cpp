#include "Object.h"

static int64_t g_CountInstanceID = 0;

Object::Object() :
	m_display_flags(true), m_instanceID((g_CountInstanceID++))
{
}

Object::~Object()
{
}

void Object::SetObjectName(const std::string& _str)
{
	m_object_name = _str;
}

void Object::DisplayFlags(bool _flag)
{
	m_display_flags = _flag;
}

void Object::Destroy(Object* _ptr)
{
	if (_ptr != nullptr)
		delete _ptr;
}
