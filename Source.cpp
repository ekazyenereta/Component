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
	// CRT���������[�N�ӏ����o
	// _CrtSetBreakAlloc(0);

	std::unique_ptr<component::Component> comonnObject(new component::Component("GameObject"));
	const std::string str_element = " num element >> ";
	int objectID = 1;

	//==========================================================================
	//
	// �I�u�W�F�N�g��ǉ����܂�
	//
	//==========================================================================
	comonnObject->AddComponent(new GameObject);
	comonnObject->AddComponent(new Transform);
	comonnObject->AddComponent(new RectTransform);
	comonnObject->AddComponent(new SampleObject);

	//==========================================================================
	//
	// �I�u�W�F�N�g���擾���悤
	//
	//==========================================================================
	 auto gameObject = comonnObject->GetComponent<GameObject>();
	 auto transform = comonnObject->GetComponent<Transform>();
	 auto rectTransform = comonnObject->GetComponent<RectTransform>();
	 auto sampleObject = comonnObject->GetComponent<SampleObject>();

	//==========================================================================
	//
	// �I�u�W�F�N�g�̓���ւ�������Ă݂悤
	//
	//==========================================================================

	 // ����ւ��邽�߂̉�����
	 auto gameObjectA = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));
	 auto gameObjectB = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));
	 
	 // sampleObject�̎q�ł��� "gameObject-2" �� "gameObject-1" �̎q�ɂ��܂�
	 gameObjectA->SetComponent(gameObjectB);

	//==========================================================================
	//
	// �I�u�W�F�N�g��j�����܂�
	//
	//==========================================================================

	 // �j���Ώۂ̉�����
	 gameObjectA = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));
	 gameObjectB = sampleObject->AddComponent(new GameObject("gameObject-" + std::to_string((objectID++))));

	 // �I�u�W�F�N�g��j������ɂ͐e�ƂȂ��Ă���@�\��������K�v������̂ňȉ��̂悤�ȋL�q�ɂȂ�܂�

	 // �e�I�u�W�F�N�g�̎擾
	 auto parA = gameObjectA->GetComponentParent();
	 // �e�����݂�������s
	 if (parA.check())
		 // �e����j���Ώۂ������܂�
		 parA->DestroyComponent(gameObjectA);

	 // namespace component ��Destroy�֐��ł��j�����\�ł�
	 component::Destroy(gameObjectB);

	//==========================================================================
	//
	// ��r���Z�q
	//
	//==========================================================================

	 if (gameObject == gameObject) {}
	 if (gameObject != gameObject) {}
	 if (gameObject == nullptr) {}
	 if (gameObject != nullptr) {}

	//==========================================================================
	//
	// ���̑�
	//
	//==========================================================================

	 // ���̂����݂��邩�̃`�F�b�N
	 if (gameObject) {}
	 if (!gameObject) {}
	 if (gameObject.check()) {}
	 if (!gameObject.check()) {}

	 // �Q�Ɛ��̎擾
	 gameObject.use_count();

	 // �Q�ƑΏۂ̎Q�Ƃ���߂܂�
	 gameObject.clear();

	//==========================================================================
	//
	// �\��
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
