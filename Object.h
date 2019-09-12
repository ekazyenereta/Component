#pragma once
#include <string>

class Object
{
public:
	Object();
	Object(const std::string& str);
	virtual ~Object();
	void SetObjectName(const std::string& _str);
	const std::string& GetObjectName();
	void DisplayFlags(bool _flag);
private:
	std::string m_object_name; // オブジェクト名
	bool m_display_flags; // オブジェクト表示フラグ
	int64_t m_instanceID; // インスタンスID
};
