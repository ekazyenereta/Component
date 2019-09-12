
#include <string>
#include "GameObject.h"
#include "Transform.h"

class Text :
	public Component
{
public:
	Text(){
		std::cout << "Text::Text()" << std::endl;
	}
	virtual ~Text() {
		std::cout << "Text::~Text()" << std::endl;
	}
};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	GameObject gameObject;

	auto* text1 = gameObject.AddComponent(new Text);
	auto* i = gameObject.GetComponent<Text>();

	auto tra = text1->GetGameObject()->GetComponent<Transform>();

	return std::system("PAUSE");
}