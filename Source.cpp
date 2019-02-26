//==========================================================================
// Source [Source.cpp]
// author: ekazyenereta
//==========================================================================
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#define _GLIBCXX_DEBUG
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Component.h"

class Test : public component::Component
{
public:
	Test() {}
	Test(const std::string & text) : m_text(text), Component(text) {}
	~Test() {}
	void print() {
		std::cout << GetComponentName() << " [test] " << m_text << std::endl;
	}
protected:
	std::string m_text;
};

class CTest : public Test
{
public:
	CTest(const std::string & text) :Test(text) {}
};

class CTest2 : public Test
{
public:
	CTest2(const std::string & text) :Test(text) {}
};

class CTest3 : public Test
{
public:
	CTest3(const std::string & text) :Test(text) {}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRTƒƒ‚ƒŠƒŠ[ƒN‰ÓŠŒŸo
	// _CrtSetBreakAlloc(0);

	std::unique_ptr<component::Component> gameObject(new component::Component("GameObject"));
	const std::string str_element = " num element >> ";

	auto obj1 = gameObject->AddComponent<CTest>("[1]");
	auto obj2 = gameObject->AddComponent<CTest2>("[2]");
	auto obj3 = gameObject->AddComponent<CTest3>("[3]");
	auto obj4 = obj3->AddComponent<CTest>("[4]");
	auto obj5 = obj3->AddComponent<CTest2>("[5]");
	auto obj6 = gameObject->GetComponent<CTest>();
	auto obj7 = gameObject->GetComponent<CTest>("[1]");
	auto obj8 = gameObject->GetComponent("[1]");
	auto obj9 = gameObject->NodeSearch<CTest2>("[8]");
	auto obj10 = gameObject->GetChild();
	auto obj11 = obj5.weak_ptr();
	auto obj12 = obj5->GetParent();

	obj12.access_count();
	obj12.check();
	obj12.weak_ptr();

	obj1->print();
	obj2->print();
	obj3->print();
	obj4->print();
	obj5->print();

	if (obj5)
	{
		std::cout << gameObject->GetComponentName() << str_element << gameObject->GetNumChild() << std::endl;
		std::cout << obj1->GetComponentName() << str_element << obj1->GetNumChild() << std::endl;
		std::cout << obj2->GetComponentName() << str_element << obj2->GetNumChild() << std::endl;
		std::cout << obj3->GetComponentName() << str_element << obj3->GetNumChild() << std::endl;
		std::cout << obj4->GetComponentName() << str_element << obj4->GetNumChild() << std::endl;
		std::cout << obj5->GetComponentName() << str_element << obj5->GetNumChild() << std::endl;
	}

	return std::system("PAUSE");
}