#pragma once
#include <string>

class Object
{
public:
	Object();
	Object(const std::string& str);
	~Object();
	void SetObjectName(const std::string& str);
	const std::string& GetObjectName();
	void DisplayFlags(bool flag);
private:
	std::string m_ObjectName; // �I�u�W�F�N�g��
	bool m_DisplayFlags; // �I�u�W�F�N�g�\���t���O
	int64_t m_InstanceID; // �C���X�^���XID
};
