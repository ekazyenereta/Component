//==========================================================================
// �R���|�[�l���g [Component.h]
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
	class Component;

	//==========================================================================
	//
	// class  : IReference 
	//
	// English
	// Content: Monitoring function reference class
	//
	// Japanese
	// Content: �Ď��@�\�̎Q�ƃN���X
	//
	//==========================================================================
	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	class IReference
	{
		static_assert(isExtended, "IReference <> : _Ty is not inherited from Component Class");
	public:
		IReference() {}
		IReference(const std::shared_ptr<Component> & _This) : m_weak(_This) {}
		IReference(const IReference  & _Right) : m_weak(_Right.m_weak.lock()) {}
		~IReference() {}

		_Ty * operator->() const noexcept {
			return (_Ty*)m_weak.lock().get();
		}
		operator bool() const noexcept {
			return !m_weak.expired();
		}
		bool operator!=(nullptr_t) const noexcept {
			return !m_weak.expired();
		}
		bool operator==(nullptr_t) const noexcept {
			return m_weak.expired();
		}
		template<class _Ty2>
		bool operator!=(IReference <_Ty2> &_Right) const noexcept {
			if (m_weak.expired())return false;
			if (_Right.m_weak.expired())return false;
			return m_weak.lock() != _Right.m_weak.lock();
		}
		template<class _Ty2>
		bool operator==(IReference <_Ty2> &_Right) const noexcept {
			if (m_weak.expired())return false;
			if (_Right.m_weak.expired())return false;
			return m_weak.lock() == _Right.m_weak.lock();
		}
		template<class _Ty2>
		IReference & operator=(IReference <_Ty2>& _Right) const noexcept {
			m_weak = _Right.weak_ptr().lock();
			return (*this);
		}

		/**
		English
		@brief checks whether the referenced object was already deleted
		@return True if the managed object exists, false otherwise
		Japanese
		@brief �Ď��Ώۂ̎����؂�⃊���N�؂�𔻒肷��
		@return �Ǘ��ΏۃI�u�W�F�N�g�����݂���ꍇ�� true�A�����łȂ��ꍇ �� false
		*/
		bool check() const noexcept {
			return !m_weak.expired();
		}

		/**
		English
		@brief returns the number of shared_ptr objects that manage the object
		@return The number of shared_ptr instances sharing the ownership of the managed object at the instant of the call.
		Japanese
		@brief �Ď����Ă���shared_ptr�I�u�W�F�N�g�̏��L�Ґ����擾����
		@return shared_ptr�Ăяo���̏u�ԂɊǗ��ΏۃI�u�W�F�N�g�̏��L�������L���Ă���C���X�^���X�̐��B
		*/
		long access_count() const noexcept {
			return m_weak.use_count();
		}

		/**
		English
		@brief Acquire resource monitoring function
		@return Resource monitoring function
		Japanese
		@brief ���\�[�X�Ď��@�\�̎擾
		@return ���\�[�X�Ď��@�\
		*/
		const std::weak_ptr<Component> & weak_ptr() const noexcept {
			return m_weak;
		}
	private:
		std::weak_ptr<Component> m_weak; // �Ď��@�\
	};

	//==========================================================================
	//
	// class  : Parent
	// Content: Parent Class
	//
	//==========================================================================
	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	class Parent
	{
	public:
		Parent() :m_parent(nullptr) {}
		Parent(Component*_Ptr) :m_parent(dynamic_cast<_Ty*>(_Ptr)) {}
		~Parent() {}

		_Ty * operator->() const noexcept {
			return m_parent;
		}
		operator bool() const noexcept {
			return m_parent != nullptr;
		}
		bool operator!=(nullptr_t) const noexcept {
			return m_parent != nullptr;
		}
		bool operator==(nullptr_t) const noexcept {
			return m_parent == nullptr;
		}
		template<class _Ty2>
		bool operator!=(Parent <_Ty2> &_Right) const noexcept {
			if (m_parent == nullptr)return false;
			if (_Right.m_parent == nullptr)return false;
			return m_parent != _Right.m_parent;
		}
		template<class _Ty2>
		bool operator==(Parent <_Ty2> &_Right) const noexcept {
			if (m_parent == nullptr)return false;
			if (_Right.m_parent == nullptr)return false;
			return m_parent == _Right.m_parent;
		}

		/**
		English
		@brief Check if parent element exists
		@return True if it exists, false if it does not exist
		Japanese
		@brief �e�v�f�����݂��邩�m�F����
		@return ���݂���ꍇ��true�A���݂��Ȃ��ꍇ��false
		*/
		bool check() const noexcept {
			return m_parent != nullptr;
		}
	private:
		_Ty * m_parent;
	};

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
		Component() : m_component_this(this), m_component_parent(nullptr) {
			size_t size = snprintf(nullptr, 0, "%p", this) + 1; // Extra space for '\0'
			std::unique_ptr<char[]> buf(new char[size]);
			snprintf(buf.get(), size, "%p", this);
			m_component_name = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
		}
		Component(const std::string & _Name) :m_component_name(_Name), m_component_this(this), m_component_parent(nullptr) {}
		virtual ~Component() {
			DestroyComponent();
			m_component_child.clear();
			m_component_name.clear();
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
			std::shared_ptr<Component> _Ptr(new _Ty());
			m_component_child.emplace_back(_Ptr);
			_Ptr->m_component_parent = this;
			return _Ptr;
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
			ptr->m_component_parent = this;
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
				if (dynamic_cast<_Ty*>(itr.get()) != nullptr)return itr;
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
				if (itr->m_component_name != _Name)continue;

				// �ΏۃR���|�[�l���g�̎擾
				if (dynamic_cast<_Ty*>(itr->m_component_this) != nullptr)
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
				if (itr->m_component_name == _Name)return itr;
			return IReference <Component>();
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
			// �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
			static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

			// �Ď��Ώۂ����݂��Ȃ��ꍇ�A���s
			if (!_Ref.check())return false;
			if (_Ref.weak_ptr().lock().get() == this)return false;

			// �Ώۂ���f�[�^���擾����
			std::shared_ptr<Component> ptr = _Ref.weak_ptr().lock();
			if (!_Ref->Aaa(_Ref))return false;
			m_component_child.emplace_back(ptr);
			ptr->m_component_parent = this;
			return true;
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
			// �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
			static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�j������
			for (auto itr = m_component_child.begin(); itr != m_component_child.end();) {
				if (dynamic_cast<_Ty*>((*itr)->m_component_this) != nullptr) {
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
			// �j���Ώۂ̌���
			auto itr = std::find(m_component_child.begin(), m_component_child.end(), _Ref.weak_ptr().lock());
			if (itr == m_component_child.end())return false;
			m_component_child.erase(itr);
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
			// �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
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
			// �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
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
				if (dynamic_cast<_Ty*>((*itr)->m_component_this) == nullptr) {
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
			std::list<IReference<Component>> Ref;
			for (auto itr : m_component_child)
				Ref.push_back(itr);
			return Ref;
		}

		/**
		English
		@brief Get parent element
		@return Parent element
		Japanese
		@brief �e�v�f���擾����
		@return �e�v�f
		*/
		template<typename _Ty = Component, bool isExtended = std::is_base_of<Component, _Ty>::value>
		Parent <_Ty> GetParent() {
			return m_component_parent;
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
			// �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
			static_assert(isExtended, "WarrantyParent<> : _Ty is not inherited from Component Class");
			IReference <_Ty> obj1 = GetComponent<_Ty>(_Name);
			if (obj1.check())return obj1;

			for (auto & itr : m_component_child) {
				IReference <_Ty> obj2 = itr->NodeSearch<_Ty>(_Name);
				if (obj2.check())return obj2;
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
		@brief Japanese : �Ǘ��������������
		@return ������������ true ���Ԃ�܂�
		*/
		template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		bool Aaa(IReference <_Ty> & _Ref) {
			if (m_component_parent == this)return false;
			auto itr = std::find(m_component_parent->m_component_child.begin(), m_component_parent->m_component_child.end(), _Ref.weak_ptr().lock());
			if (itr == m_component_parent->m_component_child.end())return false;
			m_component_parent->m_component_child.erase(itr);
			m_component_parent = nullptr;
			return true;
		}
	private:
		std::list<std::shared_ptr<Component>> m_component_child; // �R���|�[�l���g�̊Ǘ�
		Component * m_component_parent; // �e�R���|�[�l���g
		Component * m_component_this; // ����
		std::string m_component_name; // �R���|�[�l���g��
	};
}
