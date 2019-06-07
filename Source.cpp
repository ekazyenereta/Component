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

namespace text
{
	class Text : public component::Component
	{
	public:
		Text() {}
		Text(const std::string & text) : m_text(text), Component(text) {}
		~Text() {}
		void print() {
			std::cout << GetComponentName() << " [text] " << m_text << std::endl;
		}
		template <typename _Ty>
		void copy(component::Reference<_Ty> p) {
			m_text = p->m_text;
			m_text = p->m_text;
		}
	protected:
		std::string m_text;
	};

	class Text1 : public Text
	{
	public:
		Text1() :Text("text1") {}
		Text1(const std::string & text) :Text(text) {}
	};

	class Text2 : public Text
	{
	public:
		Text2() :Text("text2") {}
		Text2(const std::string & text) :Text(text) {}
	};

	class Text3 : public Text
	{
	public:
		Text3() :Text("text3") {}
		Text3(const std::string & text) :Text(text) {}
	};

	class Text4 : public component::Component
	{
	public:
		Text4() {}
		Text4(const std::string & text) : m_text(text), Component(text) {}
		~Text4() {}
		void print() {
			std::cout << GetComponentName() << " [text] " << m_text << std::endl;
		}
		template <typename _Ty>
		void copy(component::Reference<_Ty> p) {
			m_text = p->m_text;
			m_text = p->m_text;
		}
	protected:
		std::string m_text;
	};

	using ReferenceText = component::Reference<Text>;
	using ReferenceText1 = component::Reference<Text1>;
	using ReferenceText2 = component::Reference<Text2>;
	using ReferenceText3 = component::Reference<Text3>;
	using ReferenceText4 = component::Reference<Text4>;
}

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRT���������[�N�ӏ����o
	// _CrtSetBreakAlloc(0);

	std::unique_ptr<component::Component> gameObject(new component::Component("GameObject"));
	const std::string str_element = " num element >> ";

	//==========================================================================
	//
	// �I�u�W�F�N�g��ǉ����܂�
	//
	//==========================================================================
	auto text1 = gameObject->AddComponent(new text::Text1);
	auto text2 = gameObject->AddComponent(new text::Text2);
	auto text3 = gameObject->AddComponent(new text::Text3);

	// �e�L�X�g3�ɒǉ����܂�
	auto text4 = text3->AddComponent(new text::Text("text4"));
	auto text5 = text3->AddComponent(new text::Text("text5"));

	// template �Ȃ̂ŁA���̂悤�ȏ��������ł��܂�
	text3->AddComponent<text::Text>();

	// �R���X�g���N�ɉ����������������΂��̂悤�ȏ��������ł��܂�
	text3->AddComponent<text::Text>("text6");

	//==========================================================================
	//
	// �I�u�W�F�N�g���擾���悤
	//
	//==========================================================================

	// gameObject �̎q�ɑ��݂��Ă���̂Ŏ擾�ł��܂�
	auto getText1 = gameObject->GetComponent<text::Text1>();

	// text3 �ɂ� text6 �Ƃ������O�̃I�u�W�F�N�g���Ȃ��̂� nullptr ���Ԃ�܂�
	auto getText2 = text3->GetComponent("text6");

	// text3 �ɂ� Text �Ƃ����@�\�� text5 ���������O�̕t�����I�u�W�F�N�g������̂Ŏ擾�ł��܂�
	auto getText3 = text3->GetComponent<text::Text>("text5");

	//==========================================================================
	//
	// �I�u�W�F�N�g�̓���ւ�������Ă݂悤
	//
	//==========================================================================
	auto tradeObj1 = text1->AddComponent(new text::Text("trade1"));
	auto tradeObj2 = text2->AddComponent(new text::Text("trade2"));

	// trade2 �� trade1 �̎q���ɂ��܂�
	tradeObj1->SetComponent(tradeObj2);

	// trade1 �� trade2 �� text3 �̎q�ɂ��܂�
	text3->SetComponent(tradeObj1);
	text3->SetComponent(tradeObj2);

	// AddComponent �Ǝ����ǉ��̎d�����ł��܂����A�߂�l��bool�ł�
	text3->SetComponent(new text::Text("trade3"));

	//==========================================================================
	//
	// �I�u�W�F�N�g��j�����܂�
	//
	//==========================================================================
	auto desObj = text3->AddComponent(new text::Text("desObj"));
	desObj->AddComponent<text::Text1>();
	desObj->AddComponent<text::Text2>();
	desObj->AddComponent<text::Text3>();

	// �q��S�Ĕj�����܂�
	desObj->DestroyComponent();

	desObj->AddComponent(new text::Text("obj1"));
	desObj->AddComponent(new text::Text("obj2"));

	// �q�̖��O���w�肵�Ĕj�����܂�
	desObj->DestroyComponent("obj1");

	// �@�\�Ǝq�̖��O���w�肵�Ĕj�����܂�
	desObj->DestroyComponent<text::Text>("obj2");

	// �e���Ⴄ�̂Ŕj���͂ł��܂���
	if (!text1->DestroyComponent(desObj))
		// �����Ŏ����͏����܂���
		if (!desObj->DestroyComponent(desObj))
			// �e�Ȃ�������Ƃ��ł��܂�
			text3->DestroyComponent(desObj);

	desObj = text3->AddComponent(new text::Text("desObj"));
	// �e���Ăяo���āA�e�I�u�W�F�N�g����j���̏��������s���邱�Ƃ��ł��܂�
	if (desObj->GetParent() != nullptr)
		desObj->GetParent()->DestroyComponent(desObj);

	desObj = text3->AddComponent(new text::Text("desObj"));
	// �����o�ϐ��ł͂Ȃ� �j����p�֐��ɓ���邾���Ŕj�����\�ł�
	component::Destroy(desObj);

	//==========================================================================
	//
	// ��r���Z�q
	//
	//==========================================================================
	auto com1 = text3->AddComponent(new text::Text1("Comparison operator 1"));
	auto com2 = text3->AddComponent(new text::Text1("Comparison operator 2"));
	auto com3 = com1;

	auto com1_weak_ptr = com1.weak_ptr();
	auto com2_weak_ptr = com2.weak_ptr();

	auto com1_shared_ptr = com1_weak_ptr.lock();
	auto com2_shared_ptr = com2_weak_ptr.lock();

	// ����
	if (com1.check() == com3.check())
		if (com1 == com3)
			com1 = com3;

	// �����ł͂Ȃ�
	if (com1.check() == com2.check())
		if (com1 != com2)
			com1 = com2;

	// �Ǘ����Ă���f�[�^������
	if (com1)
		com1 = nullptr;

	// �Ǘ����Ă���f�[�^���Ȃ�
	if (!com1)
		com1 = com3;

	// �Ǘ����Ă���f�[�^������
	if (com1 != nullptr)
		com1 = nullptr;

	// �Ǘ����Ă���f�[�^���Ȃ�
	if (com1 == nullptr)
		com1 = com3;

	// ����
	if (com1.check())
		if (com1 == com1_weak_ptr)
			com1 = com1;
	if (com1.check())
		if (com1 == com1_shared_ptr)
			com1 = com1;

	// �����ł͂Ȃ�
	if (com1.check())
		if (com1 != com2_weak_ptr)
			com1 = com1;
	if (com1.check())
		if (com1 != com2_shared_ptr)
			com1 = com1;

	//==========================================================================
	//
	// ���̑�
	//
	//==========================================================================
	auto otherObj = text3->AddComponent(new text::Text("other"));

	// ���g���擾���܂�
	auto otherObj1 = otherObj->GameObject();
	auto otherObj2 = otherObj->GameObject<text::Text>();

	// �e���擾���܂�
	auto otherParent1 = otherObj->GetParent();
	auto otherParent2 = otherObj->GetParent<text::Text>();

	// ���O�̕ύX,�擾
	otherObj->SetComponentName("Other");
	auto otherObjName = otherObj->GetComponentName();

	// �q�̐��̎擾
	auto otherObjNumChild = otherObj->GetNumChild();

	// �q���X�g�̎擾
	auto otherObjChildList = otherObj->GetChild();

	//==========================================================================
	//
	// �\��
	//
	//==========================================================================

	// test component function
	text1->print();
	text2->print();
	text3->print();
	text4->print();
	text5->print();

	// test component function
	if (text5)
	{
		std::cout << gameObject->GetComponentName() << str_element << gameObject->GetNumChild() << std::endl;
		std::cout << text1->GetComponentName() << str_element << text1->GetNumChild() << std::endl;
		std::cout << text2->GetComponentName() << str_element << text2->GetNumChild() << std::endl;
		std::cout << text3->GetComponentName() << str_element << text3->GetNumChild() << std::endl;
		std::cout << text4->GetComponentName() << str_element << text4->GetNumChild() << std::endl;
		std::cout << text5->GetComponentName() << str_element << text5->GetNumChild() << std::endl;
	}

	// GameObject Parent null
	if(gameObject->GetParent() == nullptr)
		std::cout << "nullptr" << std::endl;

	// system
	return std::system("PAUSE");
}