#pragma once
#include <cstdio>
#include <list>
#include <unordered_map>
#include "Component.h"

class Transform;
class GameObject :
	public Object
{
private:
	class _InputComponent :
		public Component {
	public:
		_InputComponent() {}
		~_InputComponent() {}
		void SetGameObject(GameObject* _ptr) {
			m_game_object = _ptr;
		}
	};
public:
	GameObject();
	GameObject(const std::string& _str);
	virtual ~GameObject();

	/**
	English
	@brief Please add the component. Failure to inherit Component results in an error.
	@param _ptr [in] Raw pointer
	@return Monitoring function reference class
	Japanese
	@brief コンポーネントを追加してください。Componentを継承しないとエラーになります。
	@param _ptr [in] 生ポインタ
	@return 監視機能の参照クラス
	*/
	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	reference::IReference<_Ty> AddComponent(_Ty* _ptr)
	{
		static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

		// 関連付け
		((_InputComponent*)_ptr)->SetGameObject(this);
		std::shared_ptr<_Ty> ptr(_ptr);
		// クラス領域に格納します
		m_component_list[typeid(_Ty).hash_code()].emplace_back(ptr);
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
	template <typename _Ty>
	reference::IReference<_Ty> GetComponent()
	{
		// 取得対象の型があるかのチェック
		auto itr = m_component_list.find(typeid(_Ty).hash_code());
		if (itr == m_component_list.end())
			// 存在しないため空の参照機能を返します
			return reference::IReference <_Ty>();

		// 専用の管理枠があったが、実態が無い場合終了
		if ((int)itr->second.size() == 0)
			// 存在しないため空の参照機能を返します
			return reference::IReference <_Ty>();

		// 一番最後に登録されたコンポーネントを取得
		return std::dynamic_pointer_cast<_Ty>((*--itr->second.end()));
	}
protected:
	reference::IReference<Transform> m_transform;
private:
	std::unordered_map<size_t, std::list<std::shared_ptr<Component>>> m_component_list;
};

template <>
reference::IReference<Transform> GameObject::GetComponent();
