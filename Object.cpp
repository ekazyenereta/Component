#include "Object.h"

static int64_t g_CountInstanceID = 0;

Object::Object() :
	m_display_flags(true), m_instanceID((g_CountInstanceID++))
{
}

Object::Object(const std::string& str) :
	m_object_name(str)
{
}

Object::~Object()
{
}

void Object::SetObjectName(const std::string& _str)
{
	m_object_name = _str;
}

const std::string& Object::GetObjectName()
{
	return m_object_name;
}

void Object::DisplayFlags(bool _flag)
{
	m_display_flags = _flag;
}
