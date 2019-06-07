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

namespace component
{
	//==========================================================================
	//
	// class  : TemplateReference 
	//
	// English
	// Content: Monitoring function reference class
	//
	// Japanese
	// Content: 監視機能の参照クラス
	//
	//==========================================================================
	template <typename _OwnedObject, typename _BaseClass, bool isExtended = std::is_base_of<_BaseClass, _OwnedObject>::value>
	class TemplateReference
	{
		static_assert(isExtended, "TemplateReference <> : _Ty is not inherited from BaseClass Class");
	public:
		TemplateReference() {}
		TemplateReference(const std::shared_ptr<_BaseClass> & _This) : m_weak(_This) {}
		TemplateReference(const TemplateReference & _Right) : m_weak(_Right.m_weak.lock()) {}
		~TemplateReference() {}

		operator bool() const noexcept {
			return !m_weak.expired();
		}
		bool operator!=(nullptr_t) const noexcept {
			return !m_weak.expired();
		}
		bool operator==(nullptr_t) const noexcept {
			return m_weak.expired();
		}
		bool operator==(const std::shared_ptr<_BaseClass>&_Right) const noexcept {
			return m_weak.lock().get() == _Right.get();
		}
		bool operator!=(const std::shared_ptr<_BaseClass>&_Right) const noexcept {
			return m_weak.lock().get() != _Right.get();
		}
		bool operator==(const std::weak_ptr<_BaseClass>&_Right) const noexcept {
			return m_weak.lock().get() == _Right.lock().get();
		}
		bool operator!=(const std::weak_ptr<_BaseClass>&_Right) const noexcept {
			return m_weak.lock().get() != _Right.lock().get();
		}
		template<class _Ty3>
		bool operator!=(TemplateReference <_Ty3, _BaseClass> &_Right) const noexcept {
			static_assert(isExtended, "TemplateReference <> : _Ty is not inherited from BaseClass Class");

			if (m_weak.expired())
				return false;
			if (_Right.weak_ptr().expired())
				return false;
			return m_weak.lock() != _Right.weak_ptr().lock();
		}
		template<class _Ty3>
		bool operator==(TemplateReference <_Ty3, _BaseClass> &_Right) const noexcept {
			static_assert(isExtended, "TemplateReference <> : _Ty is not inherited from BaseClass Class");

			if (m_weak.expired())
				return false;
			if (_Right.weak_ptr().expired())
				return false;
			return m_weak.lock() == _Right.weak_ptr().lock();
		}
		void operator =(nullptr_t) {
			m_weak.reset();
		}

		/**
		English
		@brief checks whether the referenced object was already deleted
		@return True if the managed object exists, false otherwise
		Japanese
		@brief 監視対象の寿命切れやリンク切れを判定する
		@return 管理対象オブジェクトが存在する場合は true、そうでない場合 は false
		*/
		bool check() const noexcept {
			return !m_weak.expired();
		}

		/**
		English
		@brief returns the number of shared_ptr objects that manage the object
		@return The number of shared_ptr instances sharing the ownership of the managed object at the instant of the call.
		Japanese
		@brief 監視しているshared_ptrオブジェクトの所有者数を取得する
		@return shared_ptr呼び出しの瞬間に管理対象オブジェクトの所有権を共有しているインスタンスの数。
		*/
		long access_count() const noexcept {
			return m_weak.use_count();
		}

		/**
		English
		@brief Acquire resource monitoring function
		@return Resource monitoring function
		Japanese
		@brief リソース監視機能の取得
		@return リソース監視機能
		*/
		const std::weak_ptr<_BaseClass> & weak_ptr() const noexcept {
			return m_weak;
		}

		_OwnedObject * operator->() const noexcept {
			return (_OwnedObject*)m_weak.lock().get();
		}

		template<class _Ty3>
		bool _dynamic_cast(const _Ty3 * _Ptr) {
			return dynamic_cast<_Ty3*>(m_weak.lock().get()) == _Ptr;
		}
		template<class _Ty3>
		bool _dynamic_cast(nullptr_t) {
			return dynamic_cast<_Ty3*>(m_weak.lock().get()) == nullptr;
		}
	protected:
		std::weak_ptr<_BaseClass> m_weak; // 監視機能
	};

	class Component;
	template <typename _Ty>
	using Reference = TemplateReference<_Ty, Component>;

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
