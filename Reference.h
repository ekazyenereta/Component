//==========================================================================
// Reference [Reference.h]
// author: ekazyenereta
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#include <memory>

namespace reference
{
	template <typename _Ty>
	class IReference : 
		private std::weak_ptr<_Ty>
	{
	public:
		IReference() {}
		~IReference() {}
	private:
		// weak_ptr �̋@�\�� private �W�J���܂�
		using std::weak_ptr<_Ty>::expired;
		using std::weak_ptr<_Ty>::lock;
		using std::weak_ptr<_Ty>::reset;
	public:
		// weak_ptr �̋@�\�� public �W�J���܂�
		using std::weak_ptr<_Ty>::weak_ptr;
		using std::weak_ptr<_Ty>::use_count;
		using std::weak_ptr<_Ty>::operator=;
		using std::weak_ptr<_Ty>::element_type;

		/**
		@brief �Ď��Ώۂ̎����؂�⃊���N�؂�𔻒肷��
		@return �Ǘ��ΏۃI�u�W�F�N�g�����݂���ꍇ�� true�A�����łȂ��ꍇ �� false
		*/
		bool check() const noexcept {
			return !expired();
		}
		_Ty* operator->() const noexcept {
			return lock().get();
		}
		/**
		@brief �Ď��Ώۂ��Ď�����̂���߂܂�
		*/
		void clear() {
			reset();
		}
		void operator =(nullptr_t) {
			reset();
		}
		operator bool() const noexcept {
			return !expired();
		}
		bool operator!=(nullptr_t) const noexcept {
			return !expired();
		}
		bool operator==(nullptr_t) const noexcept {
			return expired();
		}
		bool operator==(const IReference& _Right) const noexcept {
			if (expired())
				return false;
			if (_Right.expired())
				return false;
			return lock() == _Right.lock();
		}
		bool operator!=(const IReference& _Right) const noexcept {
			if (expired())
				return false;
			if (_Right.expired())
				return false;
			return lock() != _Right.lock();
		}
	};
}