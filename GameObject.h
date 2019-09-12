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
	@brief �R���|�[�l���g��ǉ����Ă��������BComponent���p�����Ȃ��ƃG���[�ɂȂ�܂��B
	@param _ptr [in] ���|�C���^
	@return �Ď��@�\�̎Q�ƃN���X
	*/
	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	reference::IReference<_Ty> AddComponent(_Ty* _ptr)
	{
		static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

		// �֘A�t��
		((_InputComponent*)_ptr)->SetGameObject(this);
		std::shared_ptr<_Ty> ptr(_ptr);
		// �N���X�̈�Ɋi�[���܂�
		m_component_list[typeid(_Ty).hash_code()].emplace_back(ptr);
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
	reference::IReference<_Ty> GetComponent()
	{
		// �擾�Ώۂ̌^�����邩�̃`�F�b�N
		auto itr = m_component_list.find(typeid(_Ty).hash_code());
		if (itr == m_component_list.end())
			// ���݂��Ȃ����ߋ�̎Q�Ƌ@�\��Ԃ��܂�
			return reference::IReference <_Ty>();

		// ��p�̊Ǘ��g�����������A���Ԃ������ꍇ�I��
		if ((int)itr->second.size() == 0)
			// ���݂��Ȃ����ߋ�̎Q�Ƌ@�\��Ԃ��܂�
			return reference::IReference <_Ty>();

		// ��ԍŌ�ɓo�^���ꂽ�R���|�[�l���g���擾
		return std::dynamic_pointer_cast<_Ty>((*--itr->second.end()));
	}
protected:
	reference::IReference<Transform> m_transform;
private:
	std::unordered_map<size_t, std::list<std::shared_ptr<Component>>> m_component_list;
};

template <>
reference::IReference<Transform> GameObject::GetComponent();
