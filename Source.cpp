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
		void copy(component::IReference<_Ty> p) {
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
		void copy(component::IReference<_Ty> p) {
			m_text = p->m_text;
			m_text = p->m_text;
		}
	protected:
		std::string m_text;
	};

	using ReferenceText = component::IReference<Text>;
	using ReferenceText1 = component::IReference<Text1>;
	using ReferenceText2 = component::IReference<Text2>;
	using ReferenceText3 = component::IReference<Text3>;
	using ReferenceText4 = component::IReference<Text4>;
}
namespace virtual_inheritance
{
	class base : public component::Component {
	public:
		base() { std::cout << "base::base()" << std::endl; }
		base(int a) { std::cout << "base::base(" << a << ")" << std::endl; }
		void data() {
			int ins = 0;
			std::cout << "base::data()" << std::endl;
		}
		virtual ~base() {}
	};

	class derived_A : virtual public base {
	public:
		derived_A() { std::cout << "derived_A::derived_A()" << std::endl; }
	};

	class derived_B : virtual public base {
	public:
		derived_B() { std::cout << "derived_B::derived_B()" << std::endl; }
	};

	class more_derived : public derived_A, public derived_B {
	public:
		more_derived() { std::cout << "more_derived::more_derived()" << std::endl; }
	};

	template<typename T> void func(const T&) { std::cout << "func()" << std::endl; }
	template<> void func<derived_A>(const derived_A&) { std::cout << "func<derived_A>()" << std::endl; }
	template<> void func<derived_B>(const derived_B&) { std::cout << "func<derived_B>()" << std::endl; }
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
	auto text4 = text3.lock()->AddComponent(new text::Text("text4"));
	auto text5 = text3.lock()->AddComponent(new text::Text("text5"));

	//==========================================================================
	//
	// オブジェクトを取得しよう
	//
	//==========================================================================

	// gameObject の子に存在しているので取得できます
	auto getText1 = gameObject->GetComponent<text::Text1>();

	// text3 には text6 という名前のオブジェクトがないので nullptr が返ります
	auto getText2 = text3.lock()->GetComponent("text6");

	// text3 には Text という機能に text5 をいう名前の付いたオブジェクトがあるので取得できます
	auto getText3 = text3.lock()->GetComponent<text::Text>("text5");

	//==========================================================================
	//
	// オブジェクトの入れ替えをやってみよう
	//
	//==========================================================================
	auto tradeObj1 = text1.lock()->AddComponent(new text::Text("trade1"));
	auto tradeObj2 = text2.lock()->AddComponent(new text::Text("trade2"));

	// trade2 を trade1 の子供にします
	tradeObj1.lock()->SetComponent(tradeObj2);

	// trade1 と trade2 を text3 の子にします
	text3.lock()->SetComponent(tradeObj1);
	text3.lock()->SetComponent(tradeObj2);

	// AddComponent と似た追加の仕方ができますが、戻り値はboolです
	text3.lock()->SetComponent(new text::Text("trade3"));

	//==========================================================================
	//
	// オブジェクトを破棄します
	//
	//==========================================================================
	auto desObj = text3.lock()->AddComponent(new text::Text("desObj"));
	desObj.lock()->AddComponent(new text::Text1);
	desObj.lock()->AddComponent(new text::Text2);
	desObj.lock()->AddComponent(new text::Text3);

	// 子を全て破棄します
	desObj.lock()->DestroyComponent();

	desObj.lock()->AddComponent(new text::Text("obj1"));
	desObj.lock()->AddComponent(new text::Text("obj2"));

	// 子の名前を指定して破棄します
	desObj.lock()->DestroyComponent("obj1");

	// 機能と子の名前を指定して破棄します
	desObj.lock()->DestroyComponent<text::Text>("obj2");

	// 親が違うので破棄はできません
	if (!text1.lock()->DestroyComponent(desObj))
		// 自分で自分は消せません
		if (!desObj.lock()->DestroyComponent(desObj))
			// 親なら消すことができます
			text3.lock()->DestroyComponent(desObj);

	desObj = text3.lock()->AddComponent(new text::Text("desObj"));
	// 親を呼び出して、親オブジェクトから破棄の処理を実行することができます
	if (!desObj.lock()->GetComponentParent().expired())
		desObj.lock()->GetComponentParent().lock()->DestroyComponent(desObj);

	desObj = text3.lock()->AddComponent(new text::Text("desObj"));
	// メンバ変数ではない 破棄専用関数に入れるだけで破棄が可能です
	component::Destroy(desObj);

	//==========================================================================
	//
	// その他
	//
	//==========================================================================
	auto otherObj = text3.lock()->AddComponent(new text::Text("other"));

	// 自身を取得します
	auto otherObj1 = otherObj.lock()->ThisComponent();
	auto otherObj2 = otherObj.lock()->ThisComponent<text::Text>();

	// 親を取得します
	auto otherParent1 = otherObj.lock()->GetComponentParent();
	auto otherParent2 = otherObj.lock()->GetComponentParent<text::Text>();

	// 名前の変更,取得
	otherObj.lock()->SetComponentName("Other");
	auto otherObjName = otherObj.lock()->GetComponentName();

	// 子の数の取得
	auto otherObjNumChild = otherObj.lock()->GetNumChild();

	// 子リストの取得
	auto otherObjChildList = otherObj.lock()->GetComponentChild();

	//==========================================================================
	//
	// 表示
	//
	//==========================================================================

	// test component function
	text1.lock()->print();
	text2.lock()->print();
	text3.lock()->print();
	text4.lock()->print();
	text5.lock()->print();

	// test component function
	if (!text5.expired())
	{
		std::cout << gameObject->GetComponentName() << str_element << gameObject->GetNumChild() << std::endl;
		std::cout << text1.lock()->GetComponentName() << str_element << text1.lock()->GetNumChild() << std::endl;
		std::cout << text2.lock()->GetComponentName() << str_element << text2.lock()->GetNumChild() << std::endl;
		std::cout << text3.lock()->GetComponentName() << str_element << text3.lock()->GetNumChild() << std::endl;
		std::cout << text4.lock()->GetComponentName() << str_element << text4.lock()->GetNumChild() << std::endl;
		std::cout << text5.lock()->GetComponentName() << str_element << text5.lock()->GetNumChild() << std::endl;
	}

	// GameObject Parent null
	if (gameObject->GetComponentParent().expired())
		std::cout << "nullptr" << std::endl;

	std::cout << std::endl;
	std::cout << std::endl;

	gameObject->AddComponent(new virtual_inheritance::more_derived);
	gameObject->AddComponent(new virtual_inheritance::derived_A);
	gameObject->AddComponent(new virtual_inheritance::derived_B);
	gameObject->AddComponent(new virtual_inheritance::base);

	//static_cast<text::Text1*>(new text::Text1)

	// system
	return std::system("PAUSE");
}