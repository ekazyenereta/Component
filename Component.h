//==========================================================================
// コンポーネント[Component.h]
// author: ekazyenereta
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#include <cstdio>
#include <list>

//==========================================================================
// 前方
//==========================================================================
class Component;

//==========================================================================
//
// class  : GameObject
// Content: ゲームオブジェクト
//
//==========================================================================
class GameObject
{
private:
    // コピー禁止 (C++11)
    GameObject(const GameObject &) = delete;
    GameObject &operator=(const GameObject &) = delete;
public:
    GameObject() {}
    virtual ~GameObject() {
        // コンポーネントの破棄
        for (auto *& itr : m_child) {
            Destroy(itr);
        }
        m_child.clear();
    }

    /**
    @brief コンポーネントの追加。Component を継承していない場合エラーが出ます。
    @return コンポーネント
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* AddComponent() {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
        _Ty *component = new _Ty();
        component->SetGameObject(this);
        m_child.push_back(component);
        return component;
    }

    /**
    @brief コンポーネントの追加。Component を継承していない場合エラーが出ます。
    @return コンポーネント
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value, typename... _Valty>
    _Ty* AddComponent(_Valty&&... _Val) {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
        _Ty *component = new _Ty((_Val)...);
        component->SetGameObject(this);
        m_child.push_back(component);
        return component;
    }

    /**
    @brief コンポーネントの取得
    @return コンポーネント
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* GetComponent() {
        if (!isExtended)return nullptr;

        // 対象のコンポーネントが出現するまで続け、出現した場合はその実態を返す
        for (auto *& itr : m_child) {
            _Ty *component = dynamic_cast<_Ty*>(itr);
            if (component != nullptr) return component;
        }
        return dynamic_cast<_Ty*>(this);
    }

    /**
    @brief 子要素の数を返します
    @return 要素数
    */
    auto GetNumChild() const {
        return m_child.size();
    }

    /**
    @brief 子要素を返します
    @return コンポーネント格納領域
    */
    const auto GetChild() const {
        return m_child;
    }
private:
    /**
    @brief オブジェクトの破棄
    */
    template<typename _Ty>
    void Destroy(_Ty *& obj) {
        // テンプレートによるメモリ解放
        if (obj != nullptr) {
            delete obj;
            obj = nullptr;
        }
    }
private:
    std::list<Component*> m_child; // コンポーネントの管理
};

//==========================================================================
//
// class  : Component
// Content: コンポーネント
//
//==========================================================================
class Component : public GameObject
{
private:
    // コピー禁止 (C++11)
    Component(const Component &) = delete;
    Component &operator=(const Component &) = delete;
public:
    /**
    @brief コンポーネントのコンストラクタ
    */
    Component() {
        m_GameObject = nullptr;
    }

    /**
    @brief コンポーネントのデストラクタ
    */
    virtual ~Component() {
        m_GameObject = nullptr;
    }

    /**
    @brief ゲームオブジェクトの取得
    @return オブジェクト
    */
    GameObject * GetGameObject() {
        return m_GameObject;
    }

    /**
    @brief ゲームオブジェクトの登録
    */
    void SetGameObject(GameObject * obj) {
        m_GameObject = obj;
    }
private:
    GameObject* m_GameObject = nullptr; // オブジェクト
};
