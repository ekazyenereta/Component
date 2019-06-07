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
	template <typename _Ty>
	void copy(component::Reference<_Ty> p) {
		m_text = p->m_text;
		m_text = p->m_text;
	}
protected:
	std::string m_text;
};

class CTest : public Test
{
public:
	CTest() :Test("Test1") {}
	CTest(const std::string & text) :Test(text) {}
};

class CTest2 : public Test
{
public:
	CTest2() :Test("Test2") {}
	CTest2(const std::string & text) :Test(text) {}
};

class CTest3 : public Test
{
public:
	CTest3() :Test("Test3") {}
	CTest3(const std::string & text) :Test(text) {}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRTÉÅÉÇÉäÉäÅ[ÉNâ”èäåüèo
	// _CrtSetBreakAlloc(0);

	// GameObject
	std::unique_ptr<component::Component> gameObject(new component::Component("GameObject"));
	const std::string str_element = " num element >> ";

	// Add Component
	auto obj1 = gameObject->AddComponent<CTest>("[1]");
	auto obj2 = gameObject->AddComponent<CTest2>("[2]");
	auto obj3 = gameObject->AddComponent<CTest3>("[3]");
	auto obj4 = obj3->AddComponent<CTest>();
	auto obj5 = obj3->AddComponent<CTest2>();

	obj1->GameObject();

	// Get Component
	auto obj6 = gameObject->GetComponent<CTest>();
	auto obj7 = gameObject->GetComponent<CTest>("[1]");
	auto obj8 = gameObject->GetComponent("[1]");
	auto obj9 = gameObject->NodeSearch<CTest2>("[8]");

	// Get Parent
	auto obj12 = obj5->GetParent();
	auto obj13 = obj5->GetParent<CTest>();

	// Get Child
	auto obj14 = gameObject->GetChild();

	// weak_ptr
	auto obj15 = obj5.weak_ptr();

	// Object Copy
	component::Reference<CTest2> obj16 = obj5;
	auto obj17 = obj5;
	obj17 = obj16;

	// Add component
	gameObject->AddComponent(new CTest("AddComponent"));

	// Set component
	gameObject->SetComponent(new CTest("SetComponent"));
	gameObject->SetComponent(obj5);

	// Function
	obj5.access_count();
	obj5.check();
	obj5.weak_ptr();

	// test component function
	obj1->print();
	obj2->print();
	obj3->print();
	obj4->print();
	obj5->print();

	// if
	if (obj1 != obj5) {}
	if (obj1 == obj5) {}
	if (obj5 == nullptr) {}
	if (obj5 != nullptr) {}
	if (obj5) {}
	if (!obj5) {}

	obj5->GameObject()->GameObject()->GameObject()->GameObject()->GameObject()->GameObject()
		->GameObject()->GameObject()->GameObject()->GameObject()
		->GameObject()->GameObject()->GameObject()->GameObject()->GameObject()
		->GameObject()->GameObject()->GameObject()->GameObject()->GameObject()->GameObject()->GameObject()->GameObject()
		->GameObject()->GameObject();

	// test component function
	if (obj5)
	{
		std::cout << gameObject->GetComponentName() << str_element << gameObject->GetNumChild() << std::endl;
		std::cout << obj1->GetComponentName() << str_element << obj1->GetNumChild() << std::endl;
		std::cout << obj2->GetComponentName() << str_element << obj2->GetNumChild() << std::endl;
		std::cout << obj3->GetComponentName() << str_element << obj3->GetNumChild() << std::endl;
		std::cout << obj4->GetComponentName() << str_element << obj4->GetNumChild() << std::endl;
		std::cout << obj5->GetComponentName() << str_element << obj5->GetNumChild() << std::endl;
	}

	// GameObject Parent null
	if(gameObject->GetParent() == nullptr)
		std::cout << "nullptr" << std::endl;

	// null
	obj5 = nullptr;

	// system
	return std::system("PAUSE");
}