#pragma once
#include <cstdio>
#include <list>
#include <unordered_map>
#include <typeinfo>
#include "Component.h"
#include "SharePtr.h"

class Transform;
class GameObject :
	public Object
{
protected:
	Transform* m_Transform;
private:
	std::unordered_map <size_t, std::list<Component*>> m_ComponentList;
	std::list<SharePtr<GameObject>> m_GameObjectChild;
public:
	GameObject();
	GameObject(const std::string& str);
	virtual ~GameObject();

	/**
	English
	@brief Please add the component. Failure to inherit Component results in an error.
	@param ptr [in] Raw pointer
	@return Monitoring function reference class
	Japanese
	@brief コンポーネントを追加してください。Componentを継承しないとエラーになります。
	@param ptr [in] 生ポインタ
	@return 監視機能の参照クラス
	*/
	template <typename _Ty, class = std::enable_if_t<std::is_base_of<Component, _Ty>::value>>
	_Ty* AddComponent(_Ty* ptr)
	{
		// 関連付け
		static_cast<Component*>(ptr)->SetGameObject(this);
		// クラス領域に格納します
		m_ComponentList[typeid(_Ty).hash_code()].emplace_back(ptr);
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
	_Ty* GetComponent()
	{
		// 取得対象の型があるかのチェック
		auto itr = m_ComponentList.find(typeid(_Ty).hash_code());
		if (itr == m_ComponentList.end())
			return nullptr;

		// 専用の管理枠があったが、実態が無い場合終了
		if ((int)itr->second.size() == 0)
			return nullptr;

		// 一番最後に登録されたコンポーネントを取得
		return static_cast<_Ty*>((*(--itr->second.end())));
	}

	SharePtr<GameObject> AddGameObject();
	SharePtr<GameObject> AddGameObject(const std::string& str);
	SharePtr<GameObject> AddGameObject(GameObject* obj);
	const std::list<SharePtr<GameObject>>& GetChild();
	size_t GetNumChild();
};

template <>
Transform* GameObject::GetComponent();
