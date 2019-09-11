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
#include <string>

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
    Component &operator=(Component&&) = delete;
public:
    Component() {
        m_ComponentName = "default";
        m_parent = this;
    }
    virtual ~Component() {
        DestroyComponent();
        m_ComponentName.clear();
    }

    /**
    @brief コンポーネントの追加。Component を継承していない場合エラーが出ます。
    @return コンポーネント
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* AddComponent(_Ty* _Ptr) {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

		_Ptr->m_parent = this;
        m_child.push_back(_Ptr);
        return _Ptr;
    }

    /**
    @brief コンポーネントの取得
    @return コンポーネント
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* GetComponent() {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

        // 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
        for (auto *& itr : m_child) {
            _Ty *component = dynamic_cast<_Ty*>(itr);
            if (component != nullptr) return component;
        }
        return dynamic_cast<_Ty*>(this);
    }

    /**
    @brief コンポーネントの取得
    @param label [in] コンポーネント名
    @return コンポーネント
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* GetComponent(const std::string & label) {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

        // 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
        for (auto *& itr : m_child) {
            _Ty *component = dynamic_cast<_Ty*>(itr);

            // 対象コンポーネントが出現したら
            if (component != nullptr&&itr->m_ComponentName == label) {
                return component;
            }
        }
        // 上の処理で発見できなかった場合
        if (m_ComponentName == label) {
            return dynamic_cast<_Ty*>(this);
        }
        return nullptr;
    }

    /**
    @brief コンポーネントの取得、キャストはご自分でお願いします。
    @return コンポーネント
    */
    Component* GetComponent(const std::string & label) {
        // 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
        for (auto *& itr : m_child) {
            // コンポーネント名が一致した場合
            if (itr->m_ComponentName == label) {
                return itr;
            }
        }
    }

    /**
    @brief コンポーネントの登録
    @param child [in] コンポーネントの登録
    @return 登録成功時に true が返ります
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    bool SetComponent(_Ty *& child) {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

        // 登録対象が nullptr の場合、失敗
        if (child == nullptr)return false;

        // 登録対象が既に存在するかどうかチェックする
        if (!CheckComponentPointer(child)) {
            child->m_parent = this;
            m_child.push_back(child);
            return true;
        }
        return false;
    }

    /**
    @brief 指定コンポーネントの破棄
    @param label [in] コンポーネント名
    @return 破棄成功時に true が返ります
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    bool DestroyComponent(const std::string & label) {
        // コンポーネントが継承されていない場合に出力されます。
        static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");
        bool flag = false;

        // 対象のコンポーネントが出現するまで続け、出現した場合はその実体をHAL破棄する
        for (auto itr = m_child.begin();itr != m_child.end();) {
            if (dynamic_cast<_Ty*>((*itr)) != nullptr && (*itr)->m_ComponentName == label) {
                delete (*itr);
                itr = m_child.erase(itr);
                flag = true;
            }
            else {
                ++itr;
            }
        }
        return flag;
    }

    /**
    @brief コンポーネントの破棄
    */
    void DestroyComponent() {
        // 子コンポーネントの破棄
        for (auto *& itr : m_child) {
            delete itr;
            itr = nullptr;
        }
        m_child.clear();
    }

    /**
    @brief コンポーネント名の登録
    @param label [in] コンポーネント名
    */
    void SetComponentName(const std::string & label) {
        m_ComponentName = label;
    }

    /**
    @brief コンポーネント名の取得
    @return コンポーネント名
    */
    const std::string & GetComponentName() const {
        return m_ComponentName;
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
    Component * GetParent() {
        return m_parent;
    }
private:
    /**
    @brief 同一コンポーネントが存在するかどうかチェックします。存在する場合は true が返ります。
    @param input [in] チェックするコンポーネント
    */
    template <typename _Ty>
    bool CheckComponentPointer(_Ty * input) {
        for (auto *& itr : m_child) {
            _Ty * child = dynamic_cast<_Ty*>(itr);
            if (child != nullptr&&child == input) {
                return true;
            }
        }
        return false;
    }
private:
    std::list<Component*> m_child; // コンポーネントの管理
    Component* m_parent = this; // 親オブジェクト
    std::string m_ComponentName; // コンポーネント名
};

/*
_Ty(const _Ty& obj);
_Ty &operator =(const _Ty & obj);
*/
