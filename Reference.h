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
	using Reference = std::weak_ptr<_Ty>;

	template <typename _Ty>
	class IReference : private std::weak_ptr<_Ty>
	{
	public:
		IReference() {}
		~IReference() {}

		// weak_ptr の機能を展開します
		using std::weak_ptr<_Ty>::weak_ptr;
		using std::weak_ptr<_Ty>::use_count;
		using std::weak_ptr<_Ty>::operator=;
		using std::weak_ptr<_Ty>::element_type;

		/**
		English
		@brief checks whether the referenced object was already deleted
		@return True if the managed object exists, false otherwise
		Japanese
		@brief 監視対象の寿命切れやリンク切れを判定する
		@return 管理対象オブジェクトが存在する場合は true、そうでない場合 は false
		*/
		bool check() const noexcept {
			return !expired();
		}

		_Ty* operator->() const noexcept {
			return lock().get();
		}

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
	private:

	};
}