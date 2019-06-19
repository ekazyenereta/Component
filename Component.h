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

#include "Reference.h"

namespace component
{
	//==========================================================================
	//
	// class  : Component
	// Content: Component Class
	//
	//==========================================================================
	class Component
	{
	public:
		template <typename _Ty>
		using IReference = reference::TemplateReference<_Ty, Component>;
	private:
		// Copy prohibited (C++11)
		Component(const Component &) = delete;
		Component(Component&&) = delete;
		Component &operator=(const Component &) = delete;
		Component &operator=(Component&&) = delete;
	public:
		Component() : m_component_parent(nullptr) {
			size_t size = snprintf(nullptr, 0, "%p", this) + 1; // Extra space for '\0'
			std::unique_ptr<char[]> buf(new char[size]);
			snprintf(buf.get(), size, "%p", this);
			m_component_name = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
			m_component_this = std::shared_ptr<Component>(this, [](Component* p)
			{
				p = nullptr;
			});
		}
		Component(const std::string & _Name) :m_component_name(_Name), m_component_parent(nullptr) {
			m_component_this = std::shared_ptr<Component>(this, [](Component* p) 
			{
				p = nullptr; 
			});
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
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		IReference <Component> GameObject() {
			return m_component_this;
		}

		/**
		English
		@brief GameObject
		@return Monitoring function reference class
		Japanese
		@brief GameObject
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> GameObject() {
			return m_component_this;
		}

		/**
		English
		@brief Please add the component. Failure to inherit Component results in an error.
		@param _Ref [in] Raw pointer
		@return Monitoring function reference class
		Japanese
		@brief �R���|�[�l���g��ǉ����Ă��������BComponent���p�����Ȃ��ƃG���[�ɂȂ�܂��B
		@param _Ref [in] ���|�C���^
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> AddComponent(_Ty * _Ref) {
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
		@brief �R���|�[�l���g��ǉ����Ă��������BComponent���p�����Ȃ��ƃG���[�ɂȂ�܂��B
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> AddComponent() {
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
		@brief �R���|�[�l���g��ǉ����Ă��������BComponent���p�����Ȃ��ƃG���[�ɂȂ�܂��B
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value, typename... _Valty>
		IReference <_Ty> AddComponent(_Valty&&... _Val) {
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
		@brief �R���|�[�l���g�̎擾
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> GetComponent() {
			static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
			for (auto & itr : m_component_child)
				if (dynamic_cast<_Ty*>(itr.get()) != nullptr)
					return itr;
			return IReference <_Ty>();
		}

		/**
		English
		@brief Get component
		@param _Name [in] Component name
		@return Monitoring function reference class
		Japanese
		@brief �R���|�[�l���g�̎擾
		@param _Name [in] �R���|�[�l���g��
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> GetComponent(const std::string & _Name) {
			static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
			for (auto & itr : m_component_child) {
				// �ΏۃR���|�[�l���g���̎擾�Ɏ��s
				if (itr->m_component_name != _Name)
					continue;

				// �ΏۃR���|�[�l���g�̎擾
				if (dynamic_cast<_Ty*>(itr->m_component_this.get()) != nullptr)
					return itr;
			}
			return IReference <_Ty>();
		}

		/**
		English
		@brief Get component
		@param _Name [in] Component name
		@return Monitoring function reference class
		Japanese
		@brief �R���|�[�l���g�̎擾
		@param _Name [in] �R���|�[�l���g��
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		IReference <Component> GetComponent(const std::string & _Name) {
			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
			for (auto & itr : m_component_child)
				if (itr->m_component_name == _Name)
					return itr;
			return IReference <Component>();
		}

		/**
		English
		@brief Set component
		@param _Ref [in] Raw pointer
		@return Returns true on success, false on failure.
		Japanese
		@brief Set component
		@param _Ref [in] ���|�C���^
		@return ���������ꍇ��true�A���s�����ꍇ��false��Ԃ��܂��B
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
		@param _Ref [in] �Ď��@�\�̎Q�ƃN���X
		@return ���������ꍇ��true�A���s�����ꍇ��false��Ԃ��܂��B
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool SetComponent(IReference <_Ty> & _Ref) {
			static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

			// �Ď��Ώۂ����݂��Ȃ��ꍇ�A���s
			if (!_Ref.check())
				return false;
			if (_Ref->m_component_this == m_component_this)
				return false;
			// �Ώۂ̏��L�����ړ����܂�
			return _Ref->move(m_component_this);
		}

		/**
		English
		@brief Discard component
		Japanese
		@brief �R���|�[�l���g��j��
		*/
		void DestroyComponent() {
			m_component_child.clear();
		}

		/**
		English
		@brief Discard specified component
		Japanese
		@brief �w�肳�ꂽ�R���|�[�l���g��j��
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		void DestroyComponent() {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�j������
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
		@brief �w�肳�ꂽ�R���|�[�l���g��j��
		@param _Ref [in] �Ď��@�\�̎Q�ƃN���X
		@return ���������ꍇ��true�A���s�����ꍇ��false��Ԃ��܂��B
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool DestroyComponent(IReference <_Ty> & _Ref) {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			// �j���Ώۂ̌���
			auto itr = std::find(m_component_child.begin(), m_component_child.end(), _Ref->m_component_this);
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
		@brief �w�肳�ꂽ�R���|�[�l���g��j��
		@param _Name [in] �R���|�[�l���g�̖��O
		@return ���������ꍇ��true�A���s�����ꍇ��false��Ԃ��܂��B
		*/
		bool DestroyComponent(const std::string & _Name) {
			bool flag = false;
			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�j������
			for (auto itr = m_component_child.begin(); itr != m_component_child.end();) {
				// �Ώۂ̃R���|�[�l���g������v���Ȃ�
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
		@brief �w�肳�ꂽ�R���|�[�l���g��j��
		@param _Name [in] �R���|�[�l���g�̖��O
		@return ���������ꍇ��true�A���s�����ꍇ��false��Ԃ��܂��B
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool DestroyComponent(const std::string & _Name) {
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			bool flag = false;
			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�j������
			for (auto itr = m_component_child.begin(); itr != m_component_child.end();) {
				// �Ώۂ̃R���|�[�l���g������v���Ȃ�
				if ((*itr)->m_component_name != _Name) {
					++itr;
					continue;
				}
				// �Ώۂ̃R���|�[�l���g��������Ȃ�
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
		@brief �R���|�[�l���g����ݒ�
		@param _Name [in] �R���|�[�l���g�̖��O
		*/
		void SetComponentName(const std::string & _Name) {
			m_component_name = _Name;
		}

		/**
		English
		@brief Get component name
		@return Component name
		Japanese
		@brief �R���|�[�l���g�����擾
		@return �R���|�[�l���g�̖��O
		*/
		const std::string & GetComponentName() const {
			return m_component_name;
		}

		/**
		English
		@brief Get the number of child elements
		@return Number of child elements
		Japanese
		@brief �q�v�f�̐����擾����
		@return �q�v�f�̐�
		*/
		int GetNumChild() {
			return (int)m_component_child.size();
		}

		/**
		English
		@brief Get child elements
		@return Child element
		Japanese
		@brief �q�v�f���擾����
		@return �q�v�f
		*/
		const std::list<IReference<Component>> GetChild() const {
			std::list<IReference<Component>> ref;
			for (auto itr : m_component_child)
				ref.push_back(itr);
			return ref;
		}

		/**
		English
		@brief Get parent element
		@return Parent element
		Japanese
		@brief �e�v�f���擾����
		@return �e�v�f
		*/
		IReference <Component> GetParent() {
			if (m_component_parent == nullptr)
				return IReference<Component>();
			return m_component_parent;
		}

		/**
		English
		@brief Get parent element
		@return Parent element
		Japanese
		@brief �e�v�f���擾����
		@return �e�v�f
		*/
		template<typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> GetParent() {
			static_assert(isExtended, "GetParent<> : _Ty is not inherited from Component Class");

			if (m_component_parent == nullptr)
				return IReference<_Ty>();
			if (m_component_parent._dynamic_cast<_Ty>(nullptr))
				return IReference<_Ty>();
			return m_component_parent->m_component_this;
		}

		/**
		English
		@brief Search components recursively.
		@param _Name [in] Component name
		@return Monitoring function reference class
		Japanese
		@brief �R���|�[�l���g���ċA�I�Ɍ������܂��B
		@param _Name [in] �R���|�[�l���g�̖��O
		@return �Ď��@�\�̎Q�ƃN���X
		*/
		template<typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		IReference <_Ty> NodeSearch(const std::string & _Name) {
			static_assert(isExtended, "NodeSearch<> : _Ty is not inherited from Component Class");

			IReference <_Ty> obj1 = GetComponent<_Ty>(_Name);
			if (obj1.check())
				return obj1;

			for (auto & itr : m_component_child) {
				IReference <_Ty> obj2 = itr->NodeSearch<_Ty>(_Name);
				if (obj2.check())
					return obj2;
			}
			return obj1;
		}
	private:
		/**
		@brief English : Abandon administration authority
		@brief Japanese : �Ǘ��������������
		@return ������������ true ���Ԃ�܂�
		*/
		bool move(IReference<Component> _Par) {
			if (m_component_parent == nullptr)
				return false;
			if (m_component_parent == m_component_this)
				return false;
			auto itr = std::find(m_component_parent->m_component_child.begin(), m_component_parent->m_component_child.end(), m_component_this);
			if (itr == m_component_parent->m_component_child.end())
				return false;
			// �Q�Ɛ��𑝂₵�܂�
			auto ptr_copy = std::shared_ptr<Component>((*itr));
			m_component_parent->m_component_child.erase(itr);
			// �ړ���Ƀf�[�^��n���܂�
			_Par->m_component_child.emplace_back(ptr_copy);
			ptr_copy->m_component_parent = _Par->m_component_this;
			return true;
		}
	private:
		std::list<std::shared_ptr<Component>> m_component_child; // �R���|�[�l���g�̊Ǘ�
		std::shared_ptr<Component> m_component_this; // ���g
		IReference<Component> m_component_parent; // �e�R���|�[�l���g
		std::string m_component_name; // �R���|�[�l���g��
	};

	template <typename _Ty>
	using Reference = Component::IReference<_Ty>;
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
}
