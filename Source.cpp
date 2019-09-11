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

void DrawComponent(reference::IReference<component::Component> _Ref, const std::string& _Node);
void DrawDestroy(reference::IReference<component::Component> _Ref);

class GameObject :
	virtual public component::Component
{
public:
	GameObject() :Component("GameObject") {}
	GameObject(const std::string& _Name) :Component(_Name) {}
	~GameObject() {
		DrawDestroy(ThisComponent());
	}
};

class Transform :
	virtual public component::Component
{
public:
	Transform() :Component("Transform") {}
	Transform(const std::string& _Name) :Component(_Name) {}
	~Transform() {
		DrawDestroy(ThisComponent());
	}
};

class RectTransform :
	public Transform
{
public:
	RectTransform() :Component("RectTransform") {}
	RectTransform(const std::string& _Name) :Component(_Name) {}
	~RectTransform() {
		DrawDestroy(ThisComponent());
	}
};

class SampleObject :
	public Transform,
	public GameObject
{
public:
	SampleObject() :Component("SampleObject") {}
	SampleObject(const std::string& _Name) :Component(_Name) {}
	~SampleObject() {
		DrawDestroy(ThisComponent());
	}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRTメモリリーク箇所検出
	// _CrtSetBreakAlloc(0);

	std::unique_ptr<component::Component> comonnObject(new component::Component("GameObject"));
	const std::string str_element = " num element >> ";
	int objectID = 1;

	//==========================================================================
	//
	// オブジェクトを追加します
	//
	//==========================================================================
	comonnObject->AddComponent(new GameObject);
	comonnObject->AddComponent(new Transform);
	comonnObject->AddComponent(new RectTransform);
	comonnObject->AddComponent(new SampleObject);

	//==========================================================================
	//
	// オブジェクトを取得しよう
	//
	//==========================================================================
	 auto gameObject = comonnObject->GetComponent<GameObject>();
	 auto transform = comonnObject->GetComponent<Transform>();
	 auto rectTransform = comonnObject->GetComponent<RectTransform>();
	 auto sampleObject = comonnObject->GetComponent<SampleObject>();

	//==========================================================================
	//
	// オブジェクトの入れ替えをやってみよう
	//
	//==========================================================================

	 // 入れ替えるための下準備
	 auto gameObjectA = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));
	 auto gameObjectB = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));
	 
	 // sampleObjectの子である "gameObject-2" を "gameObject-1" の子にします
	 gameObjectA->SetComponent(gameObjectB);

	//==========================================================================
	//
	// オブジェクトを破棄します
	//
	//==========================================================================

	 // 破棄対象の下準備
	 gameObjectA = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));
	 gameObjectB = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));

	 // オブジェクトを破棄するには親となっている機能から消す必要があるので以下のような記述になります

	 // 親オブジェクトの取得
	 auto parA = gameObjectA->GetComponentParent();
	 // 親が存在したら実行
	 if (parA.check())
		 // 親から破棄対象を消します
		 parA->DestroyComponent(gameObjectA);

	 // namespace component のDestroy関数でも破棄が可能です
	 component::Destroy(gameObjectB);

	//==========================================================================
	//
	// 比較演算子
	//
	//==========================================================================

	 if (gameObject == gameObject) {}
	 if (gameObject != gameObject) {}
	 if (gameObject == nullptr) {}
	 if (gameObject != nullptr) {}

	//==========================================================================
	//
	// その他
	//
	//==========================================================================

	 // 実体が存在するかのチェック
	 if (gameObject) {}
	 if (!gameObject) {}
	 if (gameObject.check()) {}
	 if (!gameObject.check()) {}

	 // 参照数の取得
	 gameObject.use_count();

	 // 参照対象の参照をやめます
	 gameObject.clear();

	//==========================================================================
	//
	// 表示
	//
	//==========================================================================
	DrawComponent(comonnObject->ThisComponent(),"");

	// system
	return std::system("PAUSE");
}

void DrawComponent(reference::IReference<component::Component> _Ref, const std::string& _Node)
{
	for (auto& itr : _Ref->GetComponentChild())
	{
		auto str_node = _Node + " -> " + itr->GetComponentName();
		std::cout << str_node << std::endl;
		if (itr.check())
			DrawComponent(itr, str_node);
	}
}

void DrawDestroy(reference::IReference<component::Component> _Ref)
{
	std::string strName;
	auto par = _Ref->GetComponentParent();
	for (;;) {
		if (!par.check())
			break;
		strName = par->GetComponentName() + " -> " + strName;
		par = par->GetComponentParent();
	}
	std::cout << " [Destroy] -> " + strName + "Destroy:" + _Ref->GetComponentName() << std::endl;
}
