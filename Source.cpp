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
	// CRTメモリリーク箇所検出
	// _CrtSetBreakAlloc(0);

	std::unique_ptr<component::Component> gameObject(new component::Component("GameObject"));
	const std::string str_element = " num element >> ";

	//==========================================================================
	//
	// オブジェクトを追加します
	//
	//==========================================================================
	auto text1 = gameObject->AddComponent(new text::Text1);
	auto text2 = gameObject->AddComponent(new text::Text2);
	auto text3 = gameObject->AddComponent(new text::Text3);

	// テキスト3に追加します
	auto text4 = text3->AddComponent(new text::Text("text4"));
	auto text5 = text3->AddComponent(new text::Text("text5"));

	// template なので、このような書き方ができます
	text3->AddComponent<text::Text>();

	// コンストラクに何かしら引数があればこのような書き方ができます
	text3->AddComponent<text::Text>("text6");

	//==========================================================================
	//
	// オブジェクトを取得しよう
	//
	//==========================================================================

	// gameObject の子に存在しているので取得できます
	auto getText1 = gameObject->GetComponent<text::Text1>();

	// text3 には text6 という名前のオブジェクトがないので nullptr が返ります
	auto getText2 = text3->GetComponent("text6");

	// text3 には Text という機能に text5 をいう名前の付いたオブジェクトがあるので取得できます
	auto getText3 = text3->GetComponent<text::Text>("text5");

	//==========================================================================
	//
	// オブジェクトの入れ替えをやってみよう
	//
	//==========================================================================
	auto tradeObj1 = text1->AddComponent(new text::Text("trade1"));
	auto tradeObj2 = text2->AddComponent(new text::Text("trade2"));

	// trade2 を trade1 の子供にします
	tradeObj1->SetComponent(tradeObj2);

	// trade1 と trade2 を text3 の子にします
	text3->SetComponent(tradeObj1);
	text3->SetComponent(tradeObj2);

	// AddComponent と似た追加の仕方ができますが、戻り値はboolです
	text3->SetComponent(new text::Text("trade3"));

	//==========================================================================
	//
	// オブジェクトを破棄します
	//
	//==========================================================================
	auto desObj = text3->AddComponent(new text::Text("desObj"));
	desObj->AddComponent<text::Text1>();
	desObj->AddComponent<text::Text2>();
	desObj->AddComponent<text::Text3>();

	// 子を全て破棄します
	desObj->DestroyComponent();

	desObj->AddComponent(new text::Text("obj1"));
	desObj->AddComponent(new text::Text("obj2"));

	// 子の名前を指定して破棄します
	desObj->DestroyComponent("obj1");

	// 機能と子の名前を指定して破棄します
	desObj->DestroyComponent<text::Text>("obj2");

	// 親が違うので破棄はできません
	if (!text1->DestroyComponent(desObj))
		// 自分で自分は消せません
		if (!desObj->DestroyComponent(desObj))
			// 親なら消すことができます
			text3->DestroyComponent(desObj);

	desObj = text3->AddComponent(new text::Text("desObj"));
	// 親を呼び出して、親オブジェクトから破棄の処理を実行することができます
	if (desObj->GetParent() != nullptr)
		desObj->GetParent()->DestroyComponent(desObj);

	desObj = text3->AddComponent(new text::Text("desObj"));
	// メンバ変数ではない 破棄専用関数に入れるだけで破棄が可能です
	component::Destroy(desObj);

	//==========================================================================
	//
	// 比較演算子
	//
	//==========================================================================
	auto com1 = text3->AddComponent(new text::Text1("Comparison operator 1"));
	auto com2 = text3->AddComponent(new text::Text1("Comparison operator 2"));
	auto com3 = com1;

	auto com1_weak_ptr = com1.weak_ptr();
	auto com2_weak_ptr = com2.weak_ptr();

	auto com1_shared_ptr = com1_weak_ptr.lock();
	auto com2_shared_ptr = com2_weak_ptr.lock();

	// 同じ
	if (com1.check() == com3.check())
		if (com1 == com3)
			com1 = com3;

	// 同じではない
	if (com1.check() == com2.check())
		if (com1 != com2)
			com1 = com2;

	// 管理しているデータがある
	if (com1)
		com1 = nullptr;

	// 管理しているデータがない
	if (!com1)
		com1 = com3;

	// 管理しているデータがある
	if (com1 != nullptr)
		com1 = nullptr;

	// 管理しているデータがない
	if (com1 == nullptr)
		com1 = com3;

	// 同じ
	if (com1.check())
		if (com1 == com1_weak_ptr)
			com1 = com1;
	if (com1.check())
		if (com1 == com1_shared_ptr)
			com1 = com1;

	// 同じではない
	if (com1.check())
		if (com1 != com2_weak_ptr)
			com1 = com1;
	if (com1.check())
		if (com1 != com2_shared_ptr)
			com1 = com1;

	//==========================================================================
	//
	// その他
	//
	//==========================================================================
	auto otherObj = text3->AddComponent(new text::Text("other"));

	// 自身を取得します
	auto otherObj1 = otherObj->GameObject();
	auto otherObj2 = otherObj->GameObject<text::Text>();

	// 親を取得します
	auto otherParent1 = otherObj->GetParent();
	auto otherParent2 = otherObj->GetParent<text::Text>();

	// 名前の変更,取得
	otherObj->SetComponentName("Other");
	auto otherObjName = otherObj->GetComponentName();

	// 子の数の取得
	auto otherObjNumChild = otherObj->GetNumChild();

	// 子リストの取得
	auto otherObjChildList = otherObj->GetChild();

	//==========================================================================
	//
	// 表示
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