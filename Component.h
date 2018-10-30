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

class GameObject; // 前方

//==========================================================================
//
// class  : Component
// Content: コンポーネント
//
//==========================================================================
class Component
{
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
        // 破棄
        Destroy(m_GameObject);
    }

    /**
    @brief ゲームオブジェクトの取得
    @return オブジェクト
    */
    GameObject * GetGameObject() {
        // null の場合は実体を生成し実体を返す、実態がある場合は実体を返す
        return m_GameObject == nullptr ? m_GameObject = Initializer<GameObject>() : m_GameObject;
    }

    /**
    @brief コンポーネントの取得
    @return コンポーネント
    */
    template <typename _Ty>
    _Ty* GetComponent() {
        auto *component = dynamic_cast<_Ty*>(this);
        return component != nullptr ? component : nullptr;
    }

    /**
    @brief ゲームオブジェクトの登録
    */
    void SetGameObject(GameObject * obj) {
        m_GameObject = obj;
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

    /**
    @brief オブジェクトの生成
    @return オブジェクト
    */
    template<typename _Ty>
    _Ty * Initializer() {
        // テンプレートによるメモリ確保
        return new _Ty;
    }
private:
    GameObject* m_GameObject; // オブジェクト
};

//==========================================================================
//
// class  : GameObject
// Content: ゲームオブジェクト
//
//==========================================================================
class GameObject
{
public:
    /**
    @brief オブジェクトのコンストラクタ
    */
    GameObject() {}

    /**
    @brief オブジェクトのデストラクタ
    */
    virtual ~GameObject() {
        // コンポーネントの破棄
        for (auto * itr : m_ComponentList) {
            delete itr;
            itr = nullptr;
        }
        m_ComponentList.clear();
    }

    /**
    @brief ゲームオブジェクトの取得
    @return オブジェクト
    */
    GameObject * GetGameObject() {
        // null の場合は実体を生成し実体を返す、実態がある場合は実体を返す
        return this;
    }

    /**
    @brief コンポーネントの追加。Component を継承していない場合エラーが出ます。
    @return コンポーネント
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* AddComponent() {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
        auto *component = new _Ty();
        m_ComponentList.push_back(component);
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
        auto *component = new _Ty((_Val)...);
        m_ComponentList.push_back(component);
        return component;
    }

    /**
    @brief コンポーネントの取得
    @return コンポーネント
    */
    template <typename _Ty>
    _Ty* GetComponent() {
        // 対象のコンポーネントが出現するまで続け、出現した場合はその実態を返す
        for (auto *& itr : m_ComponentList) {
            auto *component = dynamic_cast<_Ty*>(itr);
            if (component != nullptr) return component;
        }
        return nullptr;
    }

    /**
    @brief 子要素の数を返します
    @return 要素数
    */
    auto GetNumChild() const {
        return m_ComponentList.size();
    }

    /**
    @brief 子要素を返します
    @return コンポーネント格納領域
    */
    const auto GetChild() const {
        return m_ComponentList;
    }
private:
    std::list<Component*> m_ComponentList; // コンポーネントの管理
};
