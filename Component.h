<<<<<<< HEAD
//==========================================================================
// Component [Component.h]
// author : ekazyenereta
//==========================================================================
=======
>>>>>>> Component
#pragma once
#include "Object.h"

<<<<<<< HEAD
#include <cstdio>
#include <algorithm>
#include <memory>
#include <list>
#include <unordered_map>

#include "Reference.h"

namespace component
{
	using namespace reference;

	//==========================================================================
	//
	// class : Component
	// Content : Component Class
	//
	//==========================================================================
	class Component
	{
	private:
		// Copy prohibited (C++11)
		Component(const Component&) = delete;
		Component(Component&&) = delete;
		Component& operator=(const Component&) = delete;
		Component& operator=(Component&&) = delete;
	public:
		Component() : m_component_hash_code(typeid(Component).hash_code()) {
			// アドレスのサイズを取得
			int size = snprintf(nullptr, 0, "%p", this) + 1; // Extra space for '\0'
			std::unique_ptr<char[]> buf(new char[size]);
			snprintf(buf.get(), size, "%p", this);
			// アドレスをデフォルトの名前にします
			m_component_name = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
			m_component_this = std::shared_ptr<Component>(this, [](Component* p) {p = nullptr; });
		}
		Component(const std::string& _Name) :m_component_name(_Name), m_component_hash_code(typeid(Component).hash_code()) {
			m_component_this = std::shared_ptr<Component>(this, [](Component* p) {p = nullptr; });
		}
		virtual ~Component() {
			DestroyComponent();
			m_component_child.clear();
			m_component_name.clear();
			m_component_thisptrs.clear();
		}

		/**
		English
		@brief this component
		@return Monitoring function reference class
		Japanese
		@brief this component
		@return 監視機能の参照クラス
		*/
		IReference <Component> ThisComponent() {
			return m_component_this;
		}

		/**
		English
		@brief this component
		@return Monitoring function reference class
		Japanese
		@brief this component
		@return 監視機能の参照クラス
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> ThisComponent() {
			// 指定クラスのハッシュ値で検索を行います
			auto itr1 = m_component_thisptrs.find(typeid(_Ty).hash_code());
			// そのクラスの格納領域が無かったら
			if (itr1 == m_component_thisptrs.end()) {
				// 自分自身を指定クラスでダウンキャストします
				auto ptr_this = std::dynamic_pointer_cast<_Ty>(m_component_this);
				// ダウンキャストしたが指定クラスが存在しない
				if (ptr_this == nullptr) {
					// 指定クラスが継承されていないので型領域のみ作成して終了します
					m_component_thisptrs[typeid(_Ty).hash_code()];
					return IReference <_Ty>();
				}
				else {
					// 指定クラスが存在している
					auto itr2 = m_component_thisptrs[typeid(_Ty).hash_code()] = m_component_this;
					return ptr_this;
				}
			}
			// ダウンキャストします
			return std::dynamic_pointer_cast<_Ty>(itr1->second);
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
		IReference <_Ty> AddComponent(_Ty* _Ref) {
			static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

			std::shared_ptr<_Ty> ptr(_Ref);
			// クラスのハッシュ値を生成します
			ptr->m_component_hash_code = typeid(_Ty).hash_code();
			// そのクラス領域に格納します
			m_component_child[ptr->m_component_hash_code].emplace_back(ptr);
			// 親子関係の関連付け
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
		IReference <_Ty> GetComponent() {
			static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

			// 取得対象の型があるかのチェック
			auto itr = m_component_child.find(typeid(_Ty).hash_code());
			if (itr == m_component_child.end())
				// 存在しないため空の参照機能を返します
				return IReference <_Ty>();

			// 専用の管理枠があったが、実態が無い場合終了
			if ((int)itr->second.size() == 0)
				// 存在しないため空の参照機能を返します
				return IReference <_Ty>();

			// 一番最後に登録されたコンポーネントを取得
			return std::dynamic_pointer_cast<_Ty>((*--itr->second.end()));
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
		IReference <_Ty> GetComponent(const std::string& _Name) {
			static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

			// 取得対象の型があるかのチェック
			auto itr1 = m_component_child.find(typeid(_Ty).hash_code());
			if (itr1 == m_component_child.end())
				// 存在しないため空の参照機能を返します
				return IReference <_Ty>();

			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
			for (auto& itr2 : itr1->second) {
				// 対象コンポーネント名の取得に失敗
				if (itr2->m_component_name != _Name)
					continue;

				// 対象コンポーネントの取得
				return std::dynamic_pointer_cast<_Ty>(itr2);
			}
			// 存在しないため空の参照機能を返します
			return IReference <_Ty>();
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
		IReference <Component> GetComponent(const std::string& _Name) {
			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を返す
			for (auto& itr1 : m_component_child)
				for (auto& itr2 : itr1.second)
					if (itr2->m_component_name == _Name)
						return itr2;
			// 存在しないため空の参照機能を返します
			return IReference <Component>();
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
		bool SetComponent(_Ty* _Ref) {
			static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");
			// 生のポインタが渡されたので、処理を AddComponent に委託します
			return AddComponent(_Ref).check();
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
		bool SetComponent(IReference <_Ty>& _Ref) {
			static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

			// 監視対象が存在しない場合、失敗
			if (!_Ref.check())
				return false;
			// 自身のポインタと、登録しようとしているポインタが同じなので終了
			if (_Ref->m_component_this == m_component_this)
				return false;
			// 対象の所有権を移動します
			return _Ref->move(m_component_this);
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
		@return Returns true if it succeeds, false if it fails.
		Japanese
		@brief 指定されたコンポーネントを破棄
		@return 成功した場合はtrue、失敗した場合はfalseを返します。
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool DestroyComponent() {
			(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			// 指定された型のグループを破棄する
			auto itr = m_component_child.find(typeid(_Ty).hash_code());
			// 存在しなければ終了
			if (itr == m_component_child.end())
				return false;
			// 存在するので破棄します
			m_component_child.erase(itr);
			return true;
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
		bool DestroyComponent(IReference <_Ty>& _Ref) {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			// 破棄対象の型を検索
			auto itr1 = m_component_child.find(typeid(_Ty).hash_code());
			// 存在しなければ終了
			if (itr1 == m_component_child.end())
				return false;

			// 破棄対象の検索
			auto itr2 = std::find(itr1->second.begin(), itr1->second.end(), _Ref->m_component_this);
			// 存在しなければ終了
			if (itr2 == itr1->second.end())
				return false;

			// 存在するので破棄します
			itr1->second.erase(itr2);
			_Ref.clear();
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
		bool DestroyComponent(const std::string& _Name) {
			bool flag = false;
			// 型グループを回す
			for (auto& itr1 : m_component_child)
				// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を破棄する
				for (auto itr2 = itr1.second.begin(); itr2 != itr1.second.end();) {
					// 対象のコンポーネント名が一致しない
					if ((*itr2)->m_component_name != _Name) {
						++itr2;
						continue;
					}
					itr2 = itr1.second.erase(itr2);
					flag = true;
				}
			// 成功したかどうか
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
		bool DestroyComponent(const std::string& _Name) {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			bool flag = false;

			// 破棄対象の型を検索
			auto itr1 = m_component_child.find(typeid(_Ty).hash_code());
			// 存在しなければ終了
			if (itr1 == m_component_child.end())
				return flag;

			// 対象のコンポーネントが出現するまで続け、出現した場合はその実体を破棄する
			for (auto itr2 = itr1->second.begin(); itr2 != itr1->second.end();) {
				// 対象のコンポーネント名が一致しない
				if ((*itr2)->m_component_name != _Name) {
					++itr2;
					continue;
				}
				itr2 = itr1->second.erase(itr2);
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
		void SetComponentName(const std::string& _Name) {
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
		const std::string& GetComponentName() const {
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
			int num = 0;
			// 全ての型グループに登録してあるコンポーネント数を取得します
			for (auto& itr1 : m_component_child)
				num += (int)itr1.second.size();
			return num;
		}

		/**
		English
		@brief Get child elements
		@return Child element
		Japanese
		@brief 子要素を取得する
		@return 子要素
		*/
		const std::list<IReference<Component>> GetComponentChild() const {
			std::list<IReference<Component>> ref;
			// 全てのコンポーネントを取得します
			for (auto& itr1 : m_component_child)
				for (auto& itr2 : itr1.second)
					ref.push_back(itr2);
			return ref;
		}

		/**
		English
		@brief Get child elements
		@return Child element
		Japanese
		@brief 子要素を取得する
		@return 子要素
		*/
		template<typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		const std::list<IReference<_Ty>> GetComponentChild() const {
			static_assert(isExtended, "GetComponentChild<> : _Ty is not inherited from Component Class");

			std::list<IReference<_Ty>> ref;

			// 取得対象の型があるかのチェック
			auto itr1 = m_component_child.find(typeid(_Ty).hash_code());
			if (itr1 == m_component_child.end())
				return ref;

			// 指定された型の情報を取得
			for (auto& itr2 : itr1->second)
				ref.push_back(itr2);
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
		IReference <Component> GetComponentParent() {
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
		IReference <_Ty> GetComponentParent() {
			static_assert(isExtended, "GetComponentParent<> : _Ty is not inherited from Component Class");
			// 親がいるかどうかチェックします
			if (!m_component_parent.check())
				// 存在しないため空の参照機能を返します
				return IReference<_Ty>();
			// 型指定で親のコンポーネントを取り出します
			return m_component_parent->ThisComponent<_Ty>();
		}

	private:
		/**
		@brief English : Abandon administration authority
		@brief Japanese : 管理権限を移動する
		@return 移動功時に true が返ります
		*/
		bool move(IReference<Component> _Par) {
			// 親がいるかどうかチェック
			if (!m_component_parent.check())
				return false;
			// 移動対象の実体のチェック
			if (!_Par.check())
				return false;

			// 移動したい情報の型を取得
			auto itr1 = m_component_parent->m_component_child.find(m_component_hash_code);
			// 存在しないため終了
			if (itr1 == m_component_parent->m_component_child.end())
				return false;

			// 移動対象を取得
			auto itr2 = std::find(itr1->second.begin(), itr1->second.end(), m_component_this);
			// 存在しないため終了
			if (itr2 == itr1->second.end())
				return false;

			// 参照数を増やします
			auto ptr_copy = std::shared_ptr<Component>((*itr2));
			itr1->second.erase(itr2);

			// 移動先の型領域にデータを渡します
			_Par->m_component_child[m_component_hash_code].emplace_back(ptr_copy);
			// 親子関係を結び直します
			ptr_copy->m_component_parent = _Par->m_component_this;
			// 成功
			return true;
		}
	private:
		std::unordered_map<size_t, std::list<std::shared_ptr<Component>>> m_component_child; // コンポーネントの管理
		std::unordered_map<size_t, std::shared_ptr<Component>> m_component_thisptrs; // 自身
		std::shared_ptr<Component> m_component_this; // 自身
		IReference<Component> m_component_parent; // 親コンポーネント
		std::string m_component_name; // コンポーネント名
		size_t m_component_hash_code; // コンポーネントのハッシュ
	};

	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	void Destroy(IReference<_Ty>& _Ref)
	{
		static_assert(isExtended, "Destroy <> : _Ty is not inherited from Component Class");
		// 存在するかのチェック
		if (!_Ref.check())
			return;
		// 破棄対象の親が存在するかのチェック
		if (!_Ref->GetComponentParent().check())
			return;
		// 破棄対象の親のポインタ指定の破棄処理を呼び出します
		_Ref->GetComponentParent()->DestroyComponent(_Ref);
	}
}
=======
class GameObject;
class Component :
	public Object
{
public:
	friend GameObject;
public:
	Component();
	Component(const std::string& str);
	Component(const std::string& str, GameObject* obj);
	Component(GameObject* obj);
	virtual ~Component();
private:
	void SetGameObject(GameObject* obj) {
		m_GameObject = obj;
	}
protected:
	GameObject* m_GameObject;
};
>>>>>>> Component
