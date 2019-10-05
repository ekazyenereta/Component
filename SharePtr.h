//==========================================================================
// ����X�}�[�g�|�C���^ [SharePtr.h]
// author : ekazyenereta
//==========================================================================
#pragma once
#include <type_traits>
#include <functional>

//==========================================================================
//
// class : SharePtr 
// Content : ����X�}�[�g�|�C���^
//
//==========================================================================
template <class _Ty>
class SharePtr {
private:
	// ���|�C���^�Ǘ��@�\
	class _Ptr {
	public:
		_Ty* m_data; // ���|�C���^
	private:
		std::function<void(_Ty*)> m_deleter; // �J�X�^���f���[�^
		mutable int64_t m_user_count; // count
	public:
		_Ptr() :m_data(nullptr), m_deleter([](_Ty* p) {if (p != nullptr) delete p; }), m_user_count(0) {}
		_Ptr(_Ty* _ptr) :m_data(_ptr), m_deleter([](_Ty* p) {if (p != nullptr) delete p; }), m_user_count(0) {}
		_Ptr(_Ty* _ptr, const std::function<void(_Ty*)>& _deleter) :m_data(_ptr), m_deleter(_deleter), m_user_count(0) {}
		~_Ptr() {
			destroy();
		}
		int64_t AddUserCount() {
			m_user_count++;
			return m_user_count;
		}
		int64_t GetUserCount() {
			return m_user_count;
		}
		bool Release() {
			m_user_count--;
			return m_user_count == 0 ? true : false;
		}
		void destroy() {
			m_deleter(m_data);
			m_data = nullptr;
		}
	};
private:
	_Ptr* m_ptr; // �|�C���^�Ǘ�
public:
	SharePtr() :m_ptr(nullptr) {} // empty SharePtr
	SharePtr(_Ty* _ptr) :m_ptr(new _Ptr(_ptr)) { m_ptr->AddUserCount(); }// empty SharePtr
	SharePtr(_Ty* _ptr, const std::function<void(_Ty*)>& _deleter) :m_ptr(new _Ptr(_ptr, _deleter)) { m_ptr->AddUserCount(); } // empty SharePtr
	SharePtr(nullptr_t) :m_ptr(nullptr) {} // empty SharePtr
	SharePtr(nullptr_t, const std::function<void(_Ty*)>& _deleter) :m_ptr(nullptr) {} // empty SharePtr
	SharePtr(const SharePtr& data) :m_ptr(data.m_ptr) { if (check())m_ptr->AddUserCount(); }
	~SharePtr() {
		// �Ǘ����Ă��Ȃ��ꍇ�I��
		if (m_ptr == nullptr)
			return;
		// �Q�Ɛ������炵�A����\�ł͂Ȃ��ꍇ�I��
		if (!m_ptr->Release())
			return;
		delete m_ptr;
	}
	/**
	@brief �Ď��Ώۂ̎����؂�⃊���N�؂�𔻒肷��
	@return �Ď��I�u�W�F�N�g�����݂���ꍇ�� true�A�����łȂ��ꍇ �� false
	*/
	bool check() const noexcept {
		if (m_ptr == nullptr)
			return false;
		if (m_ptr->m_data == nullptr)
			return false;
		return true;
	}
	/**
	@brief ���L���������[�U�[�����擾����B
	@return ���[�U�[��
	*/
	int64_t use_count() noexcept {
		if (check())
			return m_ptr->GetUserCount();
		return 0;
	}
	/**
	@brief �Ď��ΏۂƂ̃����N���N���A����
	*/
	void clear() noexcept {
		// �Ǘ����Ă��Ȃ��ꍇ�I��
		if (m_ptr == nullptr)
			return;
		// �Q�Ɛ������炵�������\�ɂȂ���
		if (m_ptr->Release()) {
			// �f�[�^�����݂���ꍇ���
			if (m_ptr->m_data != nullptr)
				delete m_ptr;
		}
		// �܂��Q�ƒ��̃f�[�^������̂ŁA���̊Ǘ��@�\����A�N�Z�X�ł��Ȃ��悤�ɂ���
		m_ptr = nullptr;
	}
	/**
	@brief �Ď��Ώۂ�j������
	*/
	void destroy() {
		if (check())
			m_ptr->destroy();
	}

	// return pointer to resource
	_Ty* operator->() const noexcept {
		if (check())
			return m_ptr->m_data;
		return nullptr;
	}

	// return reference to resource
	_Ty& operator*() const {
		return *m_ptr->m_data;
	}
	void operator =(nullptr_t) {
		clear();
	}
	operator bool() const noexcept {
		return check();
	}
	bool operator!=(nullptr_t) const noexcept {
		return check();
	}
	bool operator==(nullptr_t) const noexcept {
		return !check();
	}
	bool operator==(const SharePtr& _Right) const noexcept {
		if (!check())
			return false;
		if (!_Right.check())
			return false;
		return m_ptr->m_data == _Right.m_ptr->m_data;
	}
	bool operator!=(const SharePtr& _Right) const noexcept {
		if (!check())
			return false;
		if (!_Right.check())
			return false;
		return m_ptr->m_data != _Right.m_ptr->m_data;
	}
	SharePtr& operator=(const SharePtr& data) {
		auto pCopy = data.m_ptr;
		if (pCopy != nullptr)
			pCopy->AddUserCount();
		clear();
		m_ptr = pCopy;
		if (check())
			m_ptr->AddUserCount();
		if (pCopy != nullptr)
			pCopy->Release();
		return *this;
	}
};
