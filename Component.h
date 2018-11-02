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
//
// class  : Component
// Content: コンポーネント
//
//==========================================================================
class Component
{
private:
    // コピー禁止 (C++11)
    Component(const Component &) = delete;
    Component &operator=(const Component &) = delete;
public:
    Component() {
        m_GameObject = this;
    }
    virtual ~Component() {
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
        component->m_GameObject = this;
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
        component->m_GameObject = this;
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
    @brief 指定コンポーネントの破棄
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    void DestroyComponent() {
        if (isExtended) {
            // 対象のコンポーネントが出現するまで続け、出現した場合はその実態を返す
            for (auto itr = m_child.begin();itr != m_child.end();) {
                if (dynamic_cast<_Ty*>((*itr)) != nullptr) {
                    delete (*itr);
                    itr = m_child.erase(itr);
                }
                else {
                    ++itr;
                }
            }
        }
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

    /**
    @brief ゲームオブジェクトの取得
    @return オブジェクト
    */
    Component * GetGameObject() {
        return m_GameObject;
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
    Component* m_GameObject = this; // オブジェクト
};
