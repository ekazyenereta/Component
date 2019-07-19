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
		// weak_ptr の機能を private 展開します
		using std::weak_ptr<_Ty>::expired;
		using std::weak_ptr<_Ty>::lock;
		using std::weak_ptr<_Ty>::reset;
	public:
		// weak_ptr の機能を public 展開します
		using std::weak_ptr<_Ty>::weak_ptr;
		using std::weak_ptr<_Ty>::use_count;
		using std::weak_ptr<_Ty>::operator=;
		using std::weak_ptr<_Ty>::element_type;

		/**
		@brief 監視対象の寿命切れやリンク切れを判定する
		@return 管理対象オブジェクトが存在する場合は true、そうでない場合 は false
		*/
		bool check() const noexcept {
			return !expired();
		}
		_Ty* operator->() const noexcept {
			return lock().get();
		}
		/**
		@brief 監視対象を監視するのをやめます
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