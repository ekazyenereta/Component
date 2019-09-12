
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
		if (m_game_object == nullptr)
			return;
		auto itr = m_game_object->GetComponent<Transform>();
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
		if (m_game_object == nullptr)
			return;
		auto itr = m_game_object->GetComponent<Transform>();
		std::cout << itr->GetObjectName() << " -> " << GetObjectName() << std::endl;
	}
};


int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::unique_ptr<GameObject> gameObject(new GameObject("GameObject"));

	auto text = gameObject->AddComponent(new Text);
	auto t = gameObject->GetComponent<Text>();
	auto move = gameObject->AddComponent(new Move);
	auto m = gameObject->GetComponent<Move>();
	auto trans = gameObject->GetComponent<Transform>();

	t->Draw();
	m->Draw();

	return std::system("PAUSE");
}