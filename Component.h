//==========================================================================
// �R���|�[�l���g[Component.h]
// author: ekazyenereta
//==========================================================================
#pragma once

//==========================================================================
// include
//==========================================================================
#include <cstdio>
#include <list>
#include <string>

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
    Component &operator=(const Component &) = delete;
    Component &operator=(Component&&) = delete;
public:
    Component() {
        m_ComponentName = "default";
        m_GameObject = this;
    }
    virtual ~Component() {
        AllDestroyComponent();
        m_ComponentName.clear();
    }

    /**
    @brief �R���|�[�l���g�̒ǉ��BComponent ���p�����Ă��Ȃ��ꍇ�G���[���o�܂��B
    @return �R���|�[�l���g
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* AddComponent() {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

        _Ty *component = new _Ty();
        component->m_GameObject = this;
        m_child.push_back(component);
        return component;
    }

    /**
    @brief �R���|�[�l���g�̒ǉ��BComponent ���p�����Ă��Ȃ��ꍇ�G���[���o�܂��B
    @return �R���|�[�l���g
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value, typename... _Valty>
    _Ty* AddComponent(_Valty&&... _Val) {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");

        _Ty *component = new _Ty((_Val)...);
        component->m_GameObject = this;
        m_child.push_back(component);
        return component;
    }

    /**
    @brief �R���|�[�l���g�̎擾
    @return �R���|�[�l���g
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* GetComponent() {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

        // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
        for (auto *& itr : m_child) {
            _Ty *component = dynamic_cast<_Ty*>(itr);
            if (component != nullptr) return component;
        }
        return dynamic_cast<_Ty*>(this);
    }

    /**
    @brief �R���|�[�l���g�̎擾
    @param label [in] �R���|�[�l���g��
    @return �R���|�[�l���g
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* GetComponent(const std::string & label) {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "GetComponent<> : _Ty is not inherited from Component Class");

        // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
        for (auto *& itr : m_child) {
            _Ty *component = dynamic_cast<_Ty*>(itr);

            // �ΏۃR���|�[�l���g���o��������
            if (component != nullptr&&itr->m_ComponentName == label) {
                return component;
            }
        }
        // ��̏����Ŕ����ł��Ȃ������ꍇ
        if (m_ComponentName == label) {
            return dynamic_cast<_Ty*>(this);
        }
        return nullptr;
    }

    /**
    @brief �R���|�[�l���g�̎擾�A�L���X�g�͂������ł��肢���܂��B
    @return �R���|�[�l���g
    */
    Component* GetComponent(const std::string & label) {
        // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�Ԃ�
        for (auto *& itr : m_child) {
            // �R���|�[�l���g������v�����ꍇ
            if (itr->m_ComponentName == label) {
                return itr;
            }
        }
    }

    /**
    @brief �R���|�[�l���g�̓o�^
    @param child [in] �R���|�[�l���g�̓o�^
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    void SetComponent(_Ty *& child) {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "SetComponent<> : _Ty is not inherited from Component Class");

        // �o�^�Ώۂ����ɑ��݂��邩�ǂ����`�F�b�N����
        if (!CheckComponentPointer(child)) {
            child->m_GameObject = this;
            m_child.push_back(child);
        }
    }

    /**
    @brief �w��R���|�[�l���g�̔j��
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    void DestroyComponent() {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

        // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�HAL�j������
        for (auto itr = m_child.begin();itr != m_child.end();) {
            if (dynamic_cast<_Ty*>((*itr)) != nullptr) {
                delete (*itr);
                itr = m_child.erase(itr);
            }
            else {
                ++itr;
            }
        }
    }

    /**
    @brief �w��R���|�[�l���g�̔j��
    @param label [in] �R���|�[�l���g��
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    void DestroyComponent(const std::string & label) {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "DestroyComponent<> : _Ty is not inherited from Component Class");

        // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��̂�HAL�j������
        for (auto itr = m_child.begin();itr != m_child.end();) {
            if (dynamic_cast<_Ty*>((*itr)) != nullptr && (*itr)->m_ComponentName == label) {
                delete (*itr);
                itr = m_child.erase(itr);
            }
            else {
                ++itr;
            }
        }
    }

    /**
    @brief �R���|�[�l���g�̔j��
    */
    void AllDestroyComponent() {
        // �q�R���|�[�l���g�̔j��
        for (auto *& itr : m_child) {
            delete itr;
            itr = nullptr;
        }
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
    auto GetNumChild() const {
        return m_child.size();
    }

    /**
    @brief �q�v�f��Ԃ��܂�
    @return �R���|�[�l���g�i�[�̈�
    */
    const auto GetChild() const {
        return m_child;
    }

    /**
    @brief �Q�[���I�u�W�F�N�g�̎擾
    @return �I�u�W�F�N�g
    */
    Component * GetGameObject() {
        return m_GameObject;
    }
private:
    /**
    @brief ����R���|�[�l���g���ǂ����`�F�b�N���܂��B����R���|�[�l���g�̏ꍇ�� true ���Ԃ�܂��B
    @param input [in] �`�F�b�N����R���|�[�l���g
    */
    template <typename _Ty>
    bool CheckComponentPointer(_Ty * input) {
        for (auto *& itr : m_child) {
            _Ty * child = dynamic_cast<_Ty*>(itr);
            if (child != nullptr&&child == input) {
                return true;
            }
        }
        return false;
    }
private:
    std::list<Component*> m_child; // �R���|�[�l���g�̊Ǘ�
    Component* m_GameObject = this; // �I�u�W�F�N�g
    std::string m_ComponentName; // �R���|�[�l���g��
};

/*
_Ty(const _Ty& obj);
_Ty &operator =(const _Ty & obj);
*/
