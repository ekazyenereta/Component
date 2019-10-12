
#include <string>
#include <iostream>
#include "GameObject.h"
#include "Transform.h"

class Text :
	public Component
{
public:
	Text() :Component("Text") {
		std::cout << "Text::Text()" << std::endl;
	}
	virtual ~Text() {
		std::cout << "Text::~Text()" << std::endl;
	}
	void Draw() {
		if (m_GameObject == nullptr)
			return;
		auto itr = m_GameObject->GetComponent<Transform>();
		std::cout << itr->GetObjectName() << " -> " << GetObjectName() << std::endl;
	}
};

class Move :
	public Component
{
public:
	Move() :Component("Move") {
		std::cout << "Move::Move()" << std::endl;
	}
	virtual ~Move() {
		std::cout << "Move::~Move()" << std::endl;
	}
	void Draw() {
		if (m_GameObject == nullptr)
			return;
		auto itr = m_GameObject->GetComponent<Transform>();
		std::cout << itr->GetObjectName() << " -> " << GetObjectName() << std::endl;
	}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	SharePtr<GameObject> gameObject(new GameObject("GameObject"));

	auto text = gameObject->AddComponent(new Text);
	auto t = gameObject->GetComponent<Text>();
	auto move = gameObject->AddComponent(new Move);
	auto m = gameObject->GetComponent<Move>();
	auto trans = gameObject->GetComponent<Transform>();

	t->Draw();
	m->Draw();

	auto obj2 = gameObject->AddGameObject();
	auto text2 = obj2->AddComponent(new Text);
	text2->Draw();

	return std::system("PAUSE");
}