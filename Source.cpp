#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define _GLIBCXX_DEBUG
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Component.h"

class CTest : public component::Component
{
public:
    CTest() {}
    ~CTest() {}

    void Text()
    {
        printf("GetComponent >> CTest \n");
    }
private:

};

class CTest2 : public component::Component
{
public:
    CTest2() {}
    ~CTest2() {}

    void Text()
    {
        printf("GetComponent >> CTest2 \n");
    }
private:

};

class CTest3 : public component::Component
{
public:
    CTest3() {}
    ~CTest3() {}

    void Text()
    {
        printf("GetComponent >> CTest3 \n");
    }
private:

};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRTメモリリーク箇所検出
	//   _CrtSetBreakAlloc(35556);

	std::unique_ptr<component::Component> gameObject(new component::Component);

	auto obj1 = gameObject->AddComponent<CTest>();
	auto obj2 = gameObject->AddComponent<CTest2>();
	auto obj3 = gameObject->AddComponent<CTest3>();
	auto obj4 = obj3.lock()->AddComponent<CTest>();
	auto obj5 = obj3.lock()->AddComponent<CTest2>();

	obj1.lock()->Text();
	obj2.lock()->Text();
	obj3.lock()->Text();
	obj4.lock()->Text();
	obj5.lock()->Text();

	obj3.lock()->GetComponent<CTest>();
	obj3.lock()->GetComponent<CTest>("ss");
	obj3.lock()->GetComponent("ss");

	auto obj6 = obj3.lock()->GetChild();

	if (!obj5.expired())
	{
		std::cout << "CTest 参照数 >> " << obj1.lock().use_count() << std::endl;
		std::cout << "CTest2 参照数 >> " << obj2.lock().use_count() << std::endl;
		std::cout << "CTest3 参照数 >> " << obj3.lock().use_count() << std::endl;
		std::cout << "CTest 参照数 >> " << obj4.lock().use_count() << std::endl;
		std::cout << "CTest2 参照数 >> " << obj5.lock().use_count() << std::endl;
		std::cout << "gameObject 子要素の数 >> " << gameObject->GetNumChild() << std::endl;
	}


	return std::system("PAUSE");
}