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
	@brief �R���|�[�l���g��ǉ����Ă��������BComponent���p�����Ȃ��ƃG���[�ɂȂ�܂��B
	@param ptr [in] ���|�C���^
	@return �Ď��@�\�̎Q�ƃN���X
	*/
	template <typename _Ty, class = std::enable_if_t<std::is_base_of<Component, _Ty>::value>>
	_Ty* AddComponent(_Ty* ptr)
	{
		// �֘A�t��
		static_cast<Component*>(ptr)->SetGameObject(this);
		// �N���X�̈�Ɋi�[���܂�
		m_ComponentList[typeid(_Ty).hash_code()].emplace_back(ptr);
		return ptr;
	}

	/**
	English
	@brief Get component
	@return Monitoring function reference class
	Japanese
	@brief �R���|�[�l���g�̎擾
	@return �Ď��@�\�̎Q�ƃN���X
	*/
	template <typename _Ty>
	_Ty* GetComponent()
	{
		// �擾�Ώۂ̌^�����邩�̃`�F�b�N
		auto itr = m_ComponentList.find(typeid(_Ty).hash_code());
		if (itr == m_ComponentList.end())
			return nullptr;

		// ��p�̊Ǘ��g�����������A���Ԃ������ꍇ�I��
		if ((int)itr->second.size() == 0)
			return nullptr;

		// ��ԍŌ�ɓo�^���ꂽ�R���|�[�l���g���擾
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
