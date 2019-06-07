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
	// Content: 監視機能の参照クラス
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
		@brief 監視対象の寿命切れやリンク切れを判定する
		@return 管理対象オブジェクトが存在する場合は true、そうでない場合 は false
		*/
		bool check() const noexcept {
			return !m_weak.expired();
		}

		/**
		English
		@brief returns the number of shared_ptr objects that manage the object
		@return The number of shared_ptr instances sharing the ownership of the managed object at the instant of the call.
		Japanese
		@brief 監視しているshared_ptrオブジェクトの所有者数を取得する
		@return shared_ptr呼び出しの瞬間に管理対象オブジェクトの所有権を共有しているインスタンスの数。
		*/
		long access_count() const noexcept {
			return m_weak.use_count();
		}

		/**
		English
		@brief Acquire resource monitoring function
		@return Resource monitoring function
		Japanese
		@brief リソース監視機能の取得
		@return リソース監視機能
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
		std::weak_ptr<_BaseClass> m_weak; // 監視機能
	};
}