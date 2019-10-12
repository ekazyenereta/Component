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
	std::string m_ObjectName; // オブジェクト名
	bool m_DisplayFlags; // オブジェクト表示フラグ
	int64_t m_InstanceID; // インスタンスID
};
