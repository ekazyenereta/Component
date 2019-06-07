//==========================================================================
// Component [Component.h]
// author: ekazyenereta
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#include <cstdio>
#include <algorithm>
#include <memory>
#include <list>

#include "TemplateReference.h"

namespace component
{

	class Component;
	template <typename _Ty>
	using Reference = reference::TemplateReference<_Ty, Component>;

	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	void Destroy(Reference<_Ty> & _Ref)
	{
		static_assert(isExtended, "Destroy <> : _Ty is not inherited from Component Class");
		if (!_Ref.check())
			return;
		if (_Ref->GetParent() == nullptr)
			return;
		_Ref->GetParent()->DestroyComponent(_Ref);
	}

	//==========================================================================
	//
	// class  : Component
	// Content: Component Class
	//
	//==========================================================================
	class Component
	{
	private:
		// Copy prohibited (C++11)
		Component(const Component &) = delete;
		// Copy prohibited (C++11)
		Component(Component&&) = delete;
	public:
		Component() : m_component_parent(nullptr) {
			size_t size = snprintf(nullptr, 0, "%p", this) + 1; // Extra space for '\0'
			std::unique_ptr<char[]> buf(new char[size]);
			snprintf(buf.get(), size, "%p", this);
			m_component_name = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
			m_component_this = std::shared_ptr<Component>(this, [](Component* p) {p = nullptr; });
		}
		Component(const std::string & _Name) :m_component_name(_Name), m_component_parent(nullptr) {
			m_component_this = std::shared_ptr<Component>(this, [](Component* p) {p = nullptr; });
		}
		virtual ~Component() {
			DestroyComponent();
			m_component_child.clear();
			m_component_name.clear();
		}

		/**
		English
		@brief GameObject
		@return Monitoring function reference class
		Japanese
		@brief GameObject
		@return 監視機能の参照クラス
		*/
		Reference <Component> GameObject() {
			return m_component_this;
		}

		/**
		English
		@brief GameObject
		@return Monitoring function reference class
		Japanese
		@brief GameObject
		@return 監視機能の参照クラス
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Reference <_Ty> GameObject() {
			return m_component_this;
		}

		/**
		English
		@brief Please add the component. Failure to inherit Component results in an error.
		@param _Ref [in] Raw pointer
		@return Monitoring function reference class
		Japanese
		@brief コンポーネントを追加してください。Componentを継承しないとエラーになります。
		@param _Ref [in] 生ポインタ
		@return 監視機能の参照クラス
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Reference <_Ty> AddComponent(_Ty * _Ref) {
			static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

			std::shared_ptr<Component> ptr(_Ref);
			m_component_child.emplace_back(ptr);
			ptr->m_component_parent = m_component_this;
			return ptr;
		}

		/**
		English
		@brief Please add the component. Failure to inherit Component results in an error.
		@return Monitoring function reference class
		Japanese
		@brief コンポーネントを追加してください。Componentを継承しないとエラーになります。
		@return 監視機能の参照クラス
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Reference <_Ty> AddComponent() {
			static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

			std::shared_ptr<Component> ptr(new _Ty());
			m_component_child.emplace_back(ptr);
			ptr->m_component_parent = m_component_this;
			return ptr;
		}

		/**
		English
		@brief Please add the component. Failure to inherit Component results in an error.
		@return Monitoring function reference class
		Japanese
		@brief コンポーネントを追加してください。Componentを継承しないとエラーになります。
		@return 監視機能の参照クラス
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value, typename... _Valty>
		Reference <_Ty> AddComponent(_Valty&&... _Val) {
			static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

			std::shared_ptr<Component> ptr(new _Ty((_Val)...));
			m_component_child.emplace_back(ptr);
			ptr->m_component_parent = m_component_this;
			return ptr;
		}

		/**
		English
		@brief Get component
		@return Monitoring function reference class
		Japanese
		@brief コンポーネントの取得
		@return 監視機能の参照クラス
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Reference <_Ty> GetComponent() {
			static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
			for (auto & itr : m_component_child)
				if (dynamic_cast<_Ty*>(itr.get()) != nullptr)
					return itr;
			return Reference <_Ty>();
		}

		/**
		English
		@brief Get component
		@param _Name [in] Component name
		@return Monitoring function reference class
		Japanese
		@brief コンポーネントの取得
		@param _Name [in] コンポーネント名
		@return 監視機能の参照クラス
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Reference <_Ty> GetComponent(const std::string & _Name) {
			static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
			for (auto & itr : m_component_child) {
				// 対象コンポーネント名の取得に失敗
				if (itr->m_component_name != _Name)
					continue;

				// 対象コンポーネントの取得
				if (dynamic_cast<_Ty*>(itr->m_component_this.get()) != nullptr)
					return itr;
			}
			return Reference <_Ty>();
		}

		/**
		English
		@brief Get component
		@param _Name [in] Component name
		@return Monitoring function reference class
		Japanese
		@brief コンポーネントの取得
		@param _Name [in] コンポーネント名
		@return 監視機能の参照クラス
		*/
		Reference <Component> GetComponent(const std::string & _Name) {
			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
			for (auto & itr : m_component_child)
				if (itr->m_component_name == _Name)
					return itr;
			return Reference <Component>();
		}

		/**
		English
		@brief Set component
		@param _Ref [in] Raw pointer
		@return Returns true on success, false on failure.
		Japanese
		@brief Set component
		@param _Ref [in] 生ポインタ
		@return 成功した場合にtrue、失敗した場合にfalseを返します。
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool SetComponent(_Ty * _Ref) {
			static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

			return AddComponent(_Ref) != nullptr;
		}

		/**
		English
		@brief Set component
		@param _Ref [in] Monitoring function reference class
		@return Returns true on success, false on failure.
		Japanese
		@brief Set component
		@param _Ref [in] 監視機能の参照クラス
		@return 成功した場合にtrue、失敗した場合にfalseを返します。
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool SetComponent(Reference <_Ty> & _Ref) {
			static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

			// 監視対象が存在しない場合、失敗
			if (!_Ref.check())
				return false;
			if (_Ref.weak_ptr().lock() == m_component_this)
				return false;

			// 対象からデータを取得する
			std::shared_ptr<Component> ptr = _Ref.weak_ptr().lock();
			if (!_Ref->Aaa(_Ref))
				return false;
			m_component_child.emplace_back(ptr);
			ptr->m_component_parent = m_component_this;
			return true;
		}

		/**
		English
		@brief Discard component
		Japanese
		@brief コンポーネントを破棄
		*/
		void DestroyComponent() {
			m_component_child.clear();
		}

		/**
		English
		@brief Discard specified component
		Japanese
		@brief 指定されたコンポーネントを破棄
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		void DestroyComponent() {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を破棄する
			for (auto itr = m_component_child.begin(); itr != m_component_child.end();) {
				if (dynamic_cast<_Ty*>(itr->m_component_this.get()) != nullptr) {
					itr = m_component_child.erase(itr);
				}
				else {
					++itr;
				}
			}
		}

		/**
		English
		@brief Discard specified component
		@param _Ref [in] Monitoring function reference class
		@return Returns true if it succeeds, false if it fails.
		Japanese
		@brief 指定されたコンポーネントを破棄
		@param _Ref [in] 監視機能の参照クラス
		@return 成功した場合はtrue、失敗した場合はfalseを返します。
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool DestroyComponent(Reference <_Ty> & _Ref) {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			// 破棄対象の検索
			auto itr = std::find(m_component_child.begin(), m_component_child.end(), _Ref.weak_ptr().lock());
			if (itr == m_component_child.end())
				return false;
			m_component_child.erase(itr);
			_Ref = nullptr;
			return true;
		}

		/**
		English
		@brief Discard specified component
		@param _Name [in] Component name
		@return Returns true if it succeeds, false if it fails.
		Japanese
		@brief 指定されたコンポーネントを破棄
		@param _Name [in] コンポーネントの名前
		@return 成功した場合はtrue、失敗した場合はfalseを返します。
		*/
		bool DestroyComponent(const std::string & _Name) {
			bool flag = false;
			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を破棄する
			for (auto itr = m_component_child.begin(); itr != m_component_child.end();) {
				// 対象のコンポーネント名が一致しない
				if ((*itr)->m_component_name != _Name) {
					++itr;
					continue;
				}
				itr = m_component_child.erase(itr);
				flag = true;
			}
			return flag;
		}

		/**
		English
		@brief Discard specified component
		@param _Name [in] Component name
		@return Returns true if it succeeds, false if it fails.
		Japanese
		@brief 指定されたコンポーネントを破棄
		@param _Name [in] コンポーネントの名前
		@return 成功した場合はtrue、失敗した場合はfalseを返します。
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool DestroyComponent(const std::string & _Name) {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			bool flag = false;
			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を破棄する
			for (auto itr = m_component_child.begin(); itr != m_component_child.end();) {
				// 対象のコンポーネント名が一致しない
				if ((*itr)->m_component_name != _Name) {
					++itr;
					continue;
				}
				// 対象のコンポーネントが見つからない
				if (dynamic_cast<_Ty*>((*itr)->m_component_this.get()) == nullptr) {
					++itr;
					continue;
				}
				itr = m_component_child.erase(itr);
				flag = true;
			}
			return flag;
		}

		/**
		English
		@brief Set component name
		@param _Name [in] Component name
		Japanese
		@brief コンポーネント名を設定
		@param _Name [in] コンポーネントの名前
		*/
		void SetComponentName(const std::string & _Name) {
			m_component_name = _Name;
		}

		/**
		English
		@brief Get component name
		@return Component name
		Japanese
		@brief コンポーネント名を取得
		@return コンポーネントの名前
		*/
		const std::string & GetComponentName() const {
			return m_component_name;
		}

		/**
		English
		@brief Get the number of child elements
		@return Number of child elements
		Japanese
		@brief 子要素の数を取得する
		@return 子要素の数
		*/
		int GetNumChild() {
			return (int)m_component_child.size();
		}

		/**
		English
		@brief Get child elements
		@return Child element
		Japanese
		@brief 子要素を取得する
		@return 子要素
		*/
		const std::list<Reference<Component>> GetChild() const {
			std::list<Reference<Component>> ref;
			for (auto itr : m_component_child)
				ref.push_back(itr);
			return ref;
		}

		/**
		English
		@brief Get parent element
		@return Parent element
		Japanese
		@brief 親要素を取得する
		@return 親要素
		*/
		Reference <Component> GetParent() {
			if (m_component_parent == nullptr)
				return Reference<Component>();
			return m_component_parent;
		}

		/**
		English
		@brief Get parent element
		@return Parent element
		Japanese
		@brief 親要素を取得する
		@return 親要素
		*/
		template<typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Reference <_Ty> GetParent() {
			static_assert(isExtended, "GetParent<> : _Ty is not inherited from Component Class");

			if (m_component_parent == nullptr)
				return Reference<_Ty>();
			if (m_component_parent._dynamic_cast<_Ty>(nullptr))
				return Reference<_Ty>();
			return m_component_parent.weak_ptr().lock();
		}

		/**
		English
		@brief Search components recursively.
		@param _Name [in] Component name
		@return Monitoring function reference class
		Japanese
		@brief コンポーネントを再帰的に検索します。
		@param _Name [in] コンポーネントの名前
		@return 監視機能の参照クラス
		*/
		template<typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Reference <_Ty> NodeSearch(const std::string & _Name) {
			static_assert(isExtended, "NodeSearch<> : _Ty is not inherited from Component Class");

			Reference <_Ty> obj1 = GetComponent<_Ty>(_Name);
			if (obj1.check())
				return obj1;

			for (auto & itr : m_component_child) {
				Reference <_Ty> obj2 = itr->NodeSearch<_Ty>(_Name);
				if (obj2.check())
					return obj2;
			}
			return obj1;
		}

		Component &operator=(const Component & _This) {
			_This;
			return (*this);
		}

		Component &operator=(Component&& _This) {
			_This;
			return (*this);
		}
	private:
		/**
		@brief English : Abandon administration authority
		@brief Japanese : 管理権限を放棄する
		@return 放棄成功時に true が返ります
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool Aaa(Reference <_Ty> & _Ref) {
			static_assert(isExtended, "Aaa<> : _Ty is not inherited from Component Class");

			if (m_component_parent == m_component_this)
				return false;
			auto itr = std::find(m_component_parent->m_component_child.begin(), m_component_parent->m_component_child.end(), _Ref.weak_ptr().lock());
			if (itr == m_component_parent->m_component_child.end())
				return false;
			m_component_parent->m_component_child.erase(itr);
			m_component_parent = nullptr;
			return true;
		}
	private:
		std::list<std::shared_ptr<Component>> m_component_child; // コンポーネントの管理
		std::shared_ptr<Component> m_component_this; // 自身
		Reference<Component> m_component_parent; // 親コンポーネント
		std::string m_component_name; // コンポーネント名
	};
}
