//==========================================================================
// コンポーネント[Component.h]
// author: tatsuya ogawa
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#include <cstdio>
#include <algorithm>
#include <memory>
#include <list>

namespace component
{
	class Component;
	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	class Reference
	{
		static_assert(isExtended, "Reference<> : _Ty is not inherited from Component Class");
	private:
		Reference() = delete;
		Reference(Reference&&) = delete;
	public:
		Reference(const std::weak_ptr<Component> & weak) : weak(weak.lock()) {
		}
		~Reference() {
		}
		_Ty * operator->() {
			return (_Ty*)weak.lock().get();
		}
	private:
		std::weak_ptr<Component> weak; // 参照
	};
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
        Component(Component&&) = delete;
	public:
        Component() {
            size_t size = snprintf(nullptr, 0, "%p", this) + 1; // Extra space for '\0'
            std::unique_ptr<char[]> buf(new char[size]);
            snprintf(buf.get(), size, "%p", this);
            m_ComponentName = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
			m_this = std::unique_ptr<Component>(this);
		}
        virtual ~Component() {
            AllDestroyComponent();
			m_this.release();
			m_parent.release();
            m_ComponentName.clear();
			m_child.clear();
        }

        /**
        @brief コンポーネントの追加。Component を継承していない場合エラーが出ます。
        @return コンポーネント
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		std::weak_ptr<_Ty> AddComponent() {
            // コンポーネントが継承されていない場合に出力されます。
            static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
			std::shared_ptr<_Ty> ptr(new _Ty());
			m_child.emplace_back(ptr);
			ptr->m_parent = std::unique_ptr<Component>(this);
            return ptr;
        }

        /**
        @brief コンポーネントの追加。Component を継承していない場合エラーが出ます。
        @return コンポーネント
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value, typename... _Valty>
		std::weak_ptr<_Ty> AddComponent(_Valty&&... _Val) {
            // コンポーネントが継承されていない場合に出力されます。
            static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
			std::shared_ptr<_Ty> ptr(new _Ty((_Val)...));
			m_child.emplace_back(ptr);
			ptr->m_parent = std::unique_ptr<Component>(this);
            return ptr;
        }

        /**
        @brief コンポーネントの取得
        @return コンポーネント
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		std::weak_ptr<Component> GetComponent() {
            // コンポーネントが継承されていない場合に出力されます。
            static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

            // 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
            for (auto & itr : m_child)
				if (dynamic_cast<_Ty*>(itr.get()) != nullptr)return itr;

            return std::weak_ptr<Component>();
        }

        /**
        @brief コンポーネントの取得
        @param label [in] コンポーネント名
        @return コンポーネント
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		std::weak_ptr<Component> GetComponent(const std::string & label) {
            // コンポーネントが継承されていない場合に出力されます。
            static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

            // 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
            for (auto & itr : m_child) {
                // 対象コンポーネント名の取得に失敗
                if (itr->m_ComponentName != label)continue;

                // 対象コンポーネントの取得
                if (dynamic_cast<_Ty*>(itr.get()) != nullptr)
                    return itr;
            }
			return std::weak_ptr<Component>();
        }

        /**
        @brief コンポーネントの取得、キャストはご自分でお願いします。
        @return コンポーネント
        */
		std::weak_ptr<Component> GetComponent(const std::string & label) {
			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
			for (auto & itr : m_child)
				if (itr->m_ComponentName == label)return itr;
			return std::weak_ptr<Component>();
		}

        /**
        @brief コンポーネントの登録
        @param child [in] コンポーネントの登録
        @return 登録成功時に true が返ります
        */
        //template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
        bool SetComponent(std::weak_ptr<Component> & weak) {
            // コンポーネントが継承されていない場合に出力されます。
            //static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

            // 監視対象が存在しない場合、失敗
			if (weak.expired())return false;

			std::shared_ptr<Component> ptr;

			// 登録対象からデータを取得する
			weak.lock().swap(ptr);
			m_child.emplace_back(ptr);
			ptr->m_parent = std::unique_ptr<Component>(this);

            // 登録対象が既に存在するかどうかチェックする
            //if (!CheckComponentPointer(child)) {
            //    child->ThrowOutTheBox();
            //    child->m_parent = this;
            //    m_child.push_back(child);
            //    return true;
            //}
            return false;
        }

        /**
        @brief 指定コンポーネントの破棄
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
        void DestroyComponent() {
            // コンポーネントが継承されていない場合に出力されます。
            static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

            // 対象のコンポーネントが出現するまで続け、出現した場合はその実体を破棄する
			for (auto itr = m_child.begin(); itr != m_child.end();) {
				if (dynamic_cast<_Ty*>((*itr)->m_this.get()) != nullptr) {
					itr = m_child.erase(itr);
				}
				else {
					++itr;
				}
			}
        }

        /**
        @brief 指定コンポーネントの破棄
        @param ptr [in] コンポーネントの実体が入ります
        @return 破棄成功時に true が返ります
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
        bool DestroyComponent(std::weak_ptr<_Ty> & weak) {
            // コンポーネントが継承されていない場合に出力されます。
            static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

            // 破棄対象の検索
            auto itr = std::find(m_child.begin(), m_child.end(), weak.lock());
            if (itr == m_child.end())return false;
			m_child.erase(itr);
            return true;
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
			for (auto itr = m_child.begin(); itr != m_child.end();) {
				// 対象のコンポーネント名が一致しない
				if ((*itr)->m_ComponentName != label) {
					++itr;
					continue;
				}
				// 対象のコンポーネントが見つからない
				if (dynamic_cast<_Ty*>((*itr)->m_this.get()) == nullptr) {
					++itr;
					continue;
				}
				itr = m_child.erase(itr);
				flag = true;
			}
            return flag;
        }

        /**
        @brief コンポーネントの破棄
        */
        void AllDestroyComponent() {
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
        int GetNumChild()  {
            return (int)m_child.size();
        }

        /**
        @brief 子要素を返します
        @return コンポーネント格納領域
        */
        const std::list<std::weak_ptr<Component>> GetChild() const {
			std::list<std::weak_ptr<Component>> weak;
			for (auto itr : m_child)
				weak.push_back(itr);
            return weak;
        }

        /**
        @brief 親オブジェクトの取得
        @return オブジェクト
        */
		const std::unique_ptr<Component> & GetParent() {
            return m_parent;
        }

    //    /**
    //    @brief 保証された親コンポーネントの取得
    //    @description
    //    English : This is a function that guarantees that the parent component is the specified class. If it is the specified class, a pointer to the parent component is returned.
    //    Japanese : これは、親コンポーネントが指定されたクラスであることを保証する関数です。指定されたクラスである場合は、親コンポーネントのポインタが返されます。
    //    @return オブジェクト
    //    */
    //    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    //    _Ty* GetWarrantyParent() {
    //        // コンポーネントが継承されていない場合に出力されます。
    //        static_assert(isExtended, "WarrantyParent<> : _Ty is not inherited from Component Class");
    //        if (m_WarrantyParent == nullptr) {
    //            m_WarrantyParent = dynamic_cast<_Ty*>(m_parent);
    //            if (m_WarrantyParent == nullptr)
    //                m_WarrantyParent = this;
    //        }
    //        return (_Ty*)m_WarrantyParent;
    //    }

    //    /**
    //    @brief 対象コンポーネントを再帰的に探索します
    //    @return オブジェクト
    //    */
    //    template<typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    //    _Ty * NodeSearch(const std::string & name) {
    //        // コンポーネントが継承されていない場合に出力されます。
    //        static_assert(isExtended, "WarrantyParent<> : _Ty is not inherited from Component Class");
    //        _Ty * obj = GetComponent<_Ty>(name);
    //        if (obj != nullptr)return obj;
    //        for (auto & itr : GetChild()) {
    //            obj = itr->NodeSearch<_Ty>(name);
    //            if (obj != nullptr)break;
    //        }
    //        return obj;
    //    }    

    //    /**
    //    @brief コンポーネント同士のコピーを制御する演算子です
    //    */
    //    Component &operator=(const Component & obj) {
    //        obj;
    //        return *this;
    //    }
    //    /**
    //    @brief コンポーネント同士のコピーを制御する演算子です
    //    */
    //    Component &operator=(Component&& obj) {
    //        obj;
    //        return *this;
    //    }
    protected:

        /**
        @brief 同一コンポーネントが存在するかどうかチェックします。存在する場合は true が返ります。
        @param input [in] チェックするコンポーネント
        */
        template <typename _Ty>
        bool CheckComponentPointer(std::weak_ptr<_Ty> & input) {
            return std::find(m_child.begin(), m_child.end(), input) != m_child.end() ? true : false;
        }
    //private:

        /**
        @brief 親コンポーネントからこのコンポーネントを解放する
        @return 解放成功時に true が返ります
        */
        //bool ThrowOutTheBox() {
        //    if (m_parent == this)return false;
        //    auto itr = std::find(m_parent->m_child.begin(), m_parent->m_child.end(), this);
        //    if (itr == m_parent->m_child.end())return false;
        //    m_parent->m_child.erase(itr);
        //    m_parent = this;
        //    m_WarrantyParent = nullptr;
        //    return true;
        //}
    private:
        std::list<std::shared_ptr<Component>> m_child; // コンポーネントの管理
		std::unique_ptr<Component> m_parent; // 親コンポーネント
		std::unique_ptr<Component> m_this; // 自分
        std::string m_ComponentName; // コンポーネント名
    };
}
/*
_Ty(const _Ty& obj);
_Ty &operator =(const _Ty & obj);
*/
