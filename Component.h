//==========================================================================
// �R���|�[�l���g[Component.h]
// author: tatsuya ogawa
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
	template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
	class Reference
	{
		static_assert(isExtended, "Reference<> : _Ty is not inherited from Component Class");
	private:
		Reference() = delete;
		Reference(Reference&&) = delete;
	public:
		Reference(const std::weak_ptr<Component> & weak) : weak(weak.lock()) {
		}
		~Reference() {
		}
		_Ty * operator->() {
			return (_Ty*)weak.lock().get();
		}
	private:
		std::weak_ptr<Component> weak; // �Q��
	};
    //==========================================================================
    //
    // class  : Component
    // Content: �R���|�[�l���g
    //
    //==========================================================================
    class Component
    {
    private:
        // �R�s�[�֎~ (C++11)
        Component(const Component &) = delete;
        Component(Component&&) = delete;
	public:
        Component() {
            size_t size = snprintf(nullptr, 0, "%p", this) + 1; // Extra space for '\0'
            std::unique_ptr<char[]> buf(new char[size]);
            snprintf(buf.get(), size, "%p", this);
            m_ComponentName = std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
			m_this = std::unique_ptr<Component>(this);
		}
        virtual ~Component() {
            AllDestroyComponent();
			m_this.release();
			m_parent.release();
            m_ComponentName.clear();
			m_child.clear();
        }

        /**
        @brief �R���|�[�l���g�̒ǉ��BComponent ���p�����Ă��Ȃ��ꍇ�G���[���o�܂��B
        @return �R���|�[�l���g
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		std::weak_ptr<_Ty> AddComponent() {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
			std::shared_ptr<_Ty> ptr(new _Ty());
			m_child.emplace_back(ptr);
			ptr->m_parent = std::unique_ptr<Component>(this);
            return ptr;
        }

        /**
        @brief �R���|�[�l���g�̒ǉ��BComponent ���p�����Ă��Ȃ��ꍇ�G���[���o�܂��B
        @return �R���|�[�l���g
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value, typename... _Valty>
		std::weak_ptr<_Ty> AddComponent(_Valty&&... _Val) {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
			std::shared_ptr<_Ty> ptr(new _Ty((_Val)...));
			m_child.emplace_back(ptr);
			ptr->m_parent = std::unique_ptr<Component>(this);
            return ptr;
        }

        /**
        @brief �R���|�[�l���g�̎擾
        @return �R���|�[�l���g
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		std::weak_ptr<Component> GetComponent() {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

            // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
            for (auto & itr : m_child)
				if (dynamic_cast<_Ty*>(itr.get()) != nullptr)return itr;

            return std::weak_ptr<Component>();
        }

        /**
        @brief �R���|�[�l���g�̎擾
        @param label [in] �R���|�[�l���g��
        @return �R���|�[�l���g
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
		std::weak_ptr<Component> GetComponent(const std::string & label) {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

            // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
            for (auto & itr : m_child) {
                // �ΏۃR���|�[�l���g���̎擾�Ɏ��s
                if (itr->m_ComponentName != label)continue;

                // �ΏۃR���|�[�l���g�̎擾
                if (dynamic_cast<_Ty*>(itr.get()) != nullptr)
                    return itr;
            }
			return std::weak_ptr<Component>();
        }

        /**
        @brief �R���|�[�l���g�̎擾�A�L���X�g�͂������ł��肢���܂��B
        @return �R���|�[�l���g
        */
		std::weak_ptr<Component> GetComponent(const std::string & label) {
			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
			for (auto & itr : m_child)
				if (itr->m_ComponentName == label)return itr;
			return std::weak_ptr<Component>();
		}

        /**
        @brief �R���|�[�l���g�̓o�^
        @param child [in] �R���|�[�l���g�̓o�^
        @return �o�^�������� true ���Ԃ�܂�
        */
        //template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
        bool SetComponent(std::weak_ptr<Component> & weak) {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            //static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

            // �Ď��Ώۂ����݂��Ȃ��ꍇ�A���s
			if (weak.expired())return false;

			std::shared_ptr<Component> ptr;

			// �o�^�Ώۂ���f�[�^���擾����
			weak.lock().swap(ptr);
			m_child.emplace_back(ptr);
			ptr->m_parent = std::unique_ptr<Component>(this);

            // �o�^�Ώۂ����ɑ��݂��邩�ǂ����`�F�b�N����
            //if (!CheckComponentPointer(child)) {
            //    child->ThrowOutTheBox();
            //    child->m_parent = this;
            //    m_child.push_back(child);
            //    return true;
            //}
            return false;
        }

        /**
        @brief �w��R���|�[�l���g�̔j��
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
        void DestroyComponent() {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

            // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�j������
			for (auto itr = m_child.begin(); itr != m_child.end();) {
				if (dynamic_cast<_Ty*>((*itr)->m_this.get()) != nullptr) {
					itr = m_child.erase(itr);
				}
				else {
					++itr;
				}
			}
        }

        /**
        @brief �w��R���|�[�l���g�̔j��
        @param ptr [in] �R���|�[�l���g�̎��̂�����܂�
        @return �j���������� true ���Ԃ�܂�
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
        bool DestroyComponent(std::weak_ptr<_Ty> & weak) {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

            // �j���Ώۂ̌���
            auto itr = std::find(m_child.begin(), m_child.end(), weak.lock());
            if (itr == m_child.end())return false;
			m_child.erase(itr);
            return true;
        }

        /**
        @brief �w��R���|�[�l���g�̔j��
        @param label [in] �R���|�[�l���g��
        @return �j���������� true ���Ԃ�܂�
        */
        template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
        bool DestroyComponent(const std::string & label) {
            // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
            static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");
			bool flag = false;

			// �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�HAL�j������
			for (auto itr = m_child.begin(); itr != m_child.end();) {
				// �Ώۂ̃R���|�[�l���g������v���Ȃ�
				if ((*itr)->m_ComponentName != label) {
					++itr;
					continue;
				}
				// �Ώۂ̃R���|�[�l���g��������Ȃ�
				if (dynamic_cast<_Ty*>((*itr)->m_this.get()) == nullptr) {
					++itr;
					continue;
				}
				itr = m_child.erase(itr);
				flag = true;
			}
            return flag;
        }

        /**
        @brief �R���|�[�l���g�̔j��
        */
        void AllDestroyComponent() {
            m_child.clear();
        }

        /**
        @brief �R���|�[�l���g���̓o�^
        @param label [in] �R���|�[�l���g��
        */
        void SetComponentName(const std::string & label) {
            m_ComponentName = label;
        }

        /**
        @brief �R���|�[�l���g���̎擾
        @return �R���|�[�l���g��
        */
        const std::string & GetComponentName() const {
            return m_ComponentName;
        }

        /**
        @brief �q�v�f�̐���Ԃ��܂�
        @return �v�f��
        */
        int GetNumChild()  {
            return (int)m_child.size();
        }

        /**
        @brief �q�v�f��Ԃ��܂�
        @return �R���|�[�l���g�i�[�̈�
        */
        const std::list<std::weak_ptr<Component>> GetChild() const {
			std::list<std::weak_ptr<Component>> weak;
			for (auto itr : m_child)
				weak.push_back(itr);
            return weak;
        }

        /**
        @brief �e�I�u�W�F�N�g�̎擾
        @return �I�u�W�F�N�g
        */
		const std::unique_ptr<Component> & GetParent() {
            return m_parent;
        }

    //    /**
    //    @brief �ۏ؂��ꂽ�e�R���|�[�l���g�̎擾
    //    @description
    //    English : This is a function that guarantees that the parent component is the specified class. If it is the specified class, a pointer to the parent component is returned.
    //    Japanese : ����́A�e�R���|�[�l���g���w�肳�ꂽ�N���X�ł��邱�Ƃ�ۏ؂���֐��ł��B�w�肳�ꂽ�N���X�ł���ꍇ�́A�e�R���|�[�l���g�̃|�C���^���Ԃ���܂��B
    //    @return �I�u�W�F�N�g
    //    */
    //    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    //    _Ty* GetWarrantyParent() {
    //        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
    //        static_assert(isExtended, "WarrantyParent<> : _Ty is not inherited from Component Class");
    //        if (m_WarrantyParent == nullptr) {
    //            m_WarrantyParent = dynamic_cast<_Ty*>(m_parent);
    //            if (m_WarrantyParent == nullptr)
    //                m_WarrantyParent = this;
    //        }
    //        return (_Ty*)m_WarrantyParent;
    //    }

    //    /**
    //    @brief �ΏۃR���|�[�l���g���ċA�I�ɒT�����܂�
    //    @return �I�u�W�F�N�g
    //    */
    //    template<typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    //    _Ty * NodeSearch(const std::string & name) {
    //        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
    //        static_assert(isExtended, "WarrantyParent<> : _Ty is not inherited from Component Class");
    //        _Ty * obj = GetComponent<_Ty>(name);
    //        if (obj != nullptr)return obj;
    //        for (auto & itr : GetChild()) {
    //            obj = itr->NodeSearch<_Ty>(name);
    //            if (obj != nullptr)break;
    //        }
    //        return obj;
    //    }    

    //    /**
    //    @brief �R���|�[�l���g���m�̃R�s�[�𐧌䂷�鉉�Z�q�ł�
    //    */
    //    Component &operator=(const Component & obj) {
    //        obj;
    //        return *this;
    //    }
    //    /**
    //    @brief �R���|�[�l���g���m�̃R�s�[�𐧌䂷�鉉�Z�q�ł�
    //    */
    //    Component &operator=(Component&& obj) {
    //        obj;
    //        return *this;
    //    }
    protected:

        /**
        @brief ����R���|�[�l���g�����݂��邩�ǂ����`�F�b�N���܂��B���݂���ꍇ�� true ���Ԃ�܂��B
        @param input [in] �`�F�b�N����R���|�[�l���g
        */
        template <typename _Ty>
        bool CheckComponentPointer(std::weak_ptr<_Ty> & input) {
            return std::find(m_child.begin(), m_child.end(), input) != m_child.end() ? true : false;
        }
    //private:

        /**
        @brief �e�R���|�[�l���g���炱�̃R���|�[�l���g���������
        @return ����������� true ���Ԃ�܂�
        */
        //bool ThrowOutTheBox() {
        //    if (m_parent == this)return false;
        //    auto itr = std::find(m_parent->m_child.begin(), m_parent->m_child.end(), this);
        //    if (itr == m_parent->m_child.end())return false;
        //    m_parent->m_child.erase(itr);
        //    m_parent = this;
        //    m_WarrantyParent = nullptr;
        //    return true;
        //}
    private:
        std::list<std::shared_ptr<Component>> m_child; // �R���|�[�l���g�̊Ǘ�
		std::unique_ptr<Component> m_parent; // �e�R���|�[�l���g
		std::unique_ptr<Component> m_this; // ����
        std::string m_ComponentName; // �R���|�[�l���g��
    };
}
/*
_Ty(const _Ty& obj);
_Ty &operator =(const _Ty & obj);
*/
