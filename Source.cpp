
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
    GameObject gameObject;

    gameObject.AddComponent<CTest>();
    gameObject.AddComponent<CTest2>();
    gameObject.AddComponent<CTest3>()->AddComponent<CTest>()->AddComponent<CTest2>();

    printf("Žq—v‘f” >> %d \n", gameObject.GetNumChild());

    gameObject.GetComponent<CTest3>()->Text();
    gameObject.GetComponent<CTest3>()->GetComponent<CTest>()->Text();
    gameObject.GetComponent<CTest3>()->GetComponent<CTest>()->GetComponent<CTest2>()->Text();

    rewind(stdin);
    getchar();
    return 0;
}