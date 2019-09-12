#pragma once
#include <string>

class Object
{
public:
	Object();
	virtual ~Object();
	void SetObjectName(const std::string& _str);
	void DisplayFlags(bool _flag);
	static void Destroy(Object* _ptr);
private:
	std::string m_object_name; // オブジェクト名
	bool m_display_flags; // オブジェクト表示フラグ
	int64_t m_instanceID; // インスタンスID
};
