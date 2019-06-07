//==========================================================================
// TemplateReference [TemplateReference.h]
// author: ekazyenereta
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#include <memory>

namespace reference
{
	//==========================================================================
	//
	// class  : TemplateReference 
	//
	// English
	// Content: Monitoring function reference class
	//
	// Japanese
	// Content: �Ď��@�\�̎Q�ƃN���X
	//
	//==========================================================================
	template <typename _OwnedClass, typename _BaseClass, bool isExtended = std::is_base_of<_BaseClass, _OwnedClass>::value>
	class TemplateReference
	{
		static_assert(isExtended, "TemplateReference <> : _Ty is not inherited from BaseClass Class");
	public:
		TemplateReference() {}
		TemplateReference(const std::shared_ptr<_BaseClass>& _This) : m_weak(_This) {}
		TemplateReference(const TemplateReference& _Right) : m_weak(_Right.m_weak.lock()) {}
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
		bool operator==(const std::shared_ptr<_BaseClass>& _Right) const noexcept {
			return m_weak.lock().get() == _Right.get();
		}
		bool operator!=(const std::shared_ptr<_BaseClass>& _Right) const noexcept {
			return m_weak.lock().get() != _Right.get();
		}
		bool operator==(const std::weak_ptr<_BaseClass>& _Right) const noexcept {
			return m_weak.lock().get() == _Right.lock().get();
		}
		bool operator!=(const std::weak_ptr<_BaseClass>& _Right) const noexcept {
			return m_weak.lock().get() != _Right.lock().get();
		}
		template<class _Ty3>
		bool operator!=(TemplateReference <_Ty3, _BaseClass>& _Right) const noexcept {
			static_assert(isExtended, "TemplateReference <> : _Ty is not inherited from BaseClass Class");

			if (m_weak.expired())
				return false;
			if (_Right.weak_ptr().expired())
				return false;
			return m_weak.lock() != _Right.weak_ptr().lock();
		}
		template<class _Ty3>
		bool operator==(TemplateReference <_Ty3, _BaseClass>& _Right) const noexcept {
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
		const std::weak_ptr<_BaseClass>& weak_ptr() const noexcept {
			return m_weak;
		}

		_OwnedClass* operator->() const noexcept {
			return (_OwnedClass*)m_weak.lock().get();
		}

		template<class _Ty3>
		bool _dynamic_cast(const _Ty3* _Ptr) {
			return dynamic_cast<_Ty3*>(m_weak.lock().get()) == _Ptr;
		}
		template<class _Ty3>
		bool _dynamic_cast(nullptr_t) {
			return dynamic_cast<_Ty3*>(m_weak.lock().get()) == nullptr;
		}
	protected:
		std::weak_ptr<_BaseClass> m_weak; // �Ď��@�\
	};
}