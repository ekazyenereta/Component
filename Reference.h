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
	//==========================================================================
	//
	// class  : IReference 
	//
	// English
	// Content: This function is an extension of weak_ptr.
	//
	// Japanese
	// Content: weak_ptr を拡張したクラスです。
	//
	//==========================================================================
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
		@return 監視オブジェクトが存在する場合は true、そうでない場合 は false
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

	//==========================================================================
	//
	// class  : WrapperReference 
	//
	// English
	// Content: Monitoring function reference class
	//
	// Japanese
	// Content: 監視機能の参照クラス
	//
	//==========================================================================
	template <typename _Derived, typename _Base, bool isExtended = std::is_base_of<_Base, _Derived>::value>
	class WrapperReference
	{
		static_assert(isExtended, "WrapperReference <> : _Ty is not inherited from BaseClass Class");
	public:
		using Owned = _Derived;
	public:
		WrapperReference() {}
		WrapperReference(const std::shared_ptr<_Base>& _This) : m_weak(_This) {}
		WrapperReference(const IReference<_Base>& _This) : m_weak(_This) {}
		WrapperReference(const WrapperReference& _Right) : m_weak(_Right.m_weak) {}
		~WrapperReference() {}

		operator bool() const noexcept {
			return m_weak.check();
		}
		bool operator!=(nullptr_t) const noexcept {
			return m_weak != nullptr;
		}
		bool operator==(nullptr_t) const noexcept {
			return m_weak == nullptr;
		}
		bool operator==(const IReference<_Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!_Right.check())
				return false;
			return m_weak == _Right;
		}
		bool operator!=(const IReference<_Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!m_weak.check())
				return false;
			return m_weak != _Right;
		}
		template<class _Ty>
		bool operator!=(WrapperReference <_Ty, _Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!m_weak.check())
				return false;
			return m_weak != _Right.m_weak;
		}
		template<class _Ty>
		bool operator==(WrapperReference <_Ty, _Base>& _Right) const noexcept {
			if (!m_weak.check())
				return false;
			if (!m_weak.check())
				return false;
			return m_weak == _Right.m_weak;
		}
		void operator =(nullptr_t) {
			m_weak.clear();
		}
		auto* operator->() const noexcept {
			return (_Derived*)m_weak.operator->();
		}

		/**
		English
		@brief checks whether the referenced object was already deleted
		@return True if the managed object exists, false otherwise
		Japanese
		@brief 監視対象の寿命切れやリンク切れを判定する
		@return 監視オブジェクトが存在する場合は true、そうでない場合 は false
		*/
		bool check() const noexcept {
			return m_weak.check();
		}

		/**
		English
		@brief returns the number of shared_ptr objects that manage the object
		@return The number of shared_ptr instances sharing the ownership of the managed object at the instant of the call.
		Japanese
		@brief 監視しているshared_ptrオブジェクトの所有者数を取得する
		@return shared_ptr呼び出しの瞬間に管理対象オブジェクトの所有権を共有しているインスタンスの数。
		*/
		long use_count() const noexcept {
			return m_weak.use_count();
		}
		/**
		@brief 監視対象を監視するのをやめます
		*/
		void clear() {
			m_weak.clear();
		}
	protected:
		IReference<_Base> m_weak; // 監視機能
	};
}
