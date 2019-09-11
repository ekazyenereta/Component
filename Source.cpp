
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

#include "Component.h"

class CTest : public Component
{
public:
    CTest() {}
    ~CTest() {}

    void Text()
    {
        printf("GetComponent >> CTest \n");
    }
private:

};

class CTest2 : public Component
{
public:
    CTest2() {}
    ~CTest2() {}

    void Text()
    {
        printf("GetComponent >> CTest2 \n");
    }
private:

};

class CTest3 : public Component
{
public:
    CTest3() {}
    ~CTest3() {}

    void Text()
    {
        printf("GetComponent >> CTest3 \n");
    }
private:

};

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// CRTメモリリーク箇所検出
	// _CrtSetBreakAlloc(0);

    Component gameObject;

    gameObject.AddComponent(new CTest);
    gameObject.AddComponent(new CTest2);
    gameObject.GetComponent<CTest2>()->SetComponentName("test");
    gameObject.AddComponent(new CTest3)->AddComponent(new CTest)->AddComponent(new CTest2);

    printf("子要素数 >> %d \n", gameObject.GetNumChild());

    gameObject.GetComponent<CTest3>()->Text();
    gameObject.GetComponent<CTest3>()->GetComponent<CTest>()->Text();
    gameObject.GetComponent<CTest3>()->GetComponent<CTest>()->GetComponent<CTest2>()->Text();

    if (gameObject.GetComponent<CTest2>("test") != nullptr) {
        gameObject.GetComponent<CTest2>("test")->Text();
    }
    else if (gameObject.GetComponent<CTest2>("test") == nullptr) {
        printf("not name");
    }

    rewind(stdin);
    getchar();
    return 0;
}