#include "Object.h"

static int64_t g_CountInstanceID = 0;

Object::Object() :
	m_DisplayFlags(true), m_InstanceID((g_CountInstanceID++))
{
}

Object::Object(const std::string& str) :
	m_DisplayFlags(true), m_ObjectName(str), m_InstanceID((g_CountInstanceID++))
{
}

Object::~Object()
{
}

void Object::SetObjectName(const std::string& str)
{
	m_ObjectName = str;
}

const std::string& Object::GetObjectName()
{
	return m_ObjectName;
}

void Object::DisplayFlags(bool flag)
{
	m_DisplayFlags = flag;
}
