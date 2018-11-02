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
        m_GameObject = this;
    }
    virtual ~Component() {
        AllDestroyComponent();
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
    @brief �R���|�[�l���g�̓o�^
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    void SetComponent(_Ty *& child) {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "SetChild<> : _Ty is not inherited from Component Class");

        child->m_GameObject = this;
        m_child.push_back(child);
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
    @brief �R���|�[�l���g�̔j��
    */
    void AllDestroyComponent() {
        // �q�R���|�[�l���g�̔j��
        for (auto itr = m_child.begin();itr != m_child.end();++itr) {
            delete (*itr);
            (*itr) = nullptr;
        }
        m_child.clear();
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
    std::list<Component*> m_child; // �R���|�[�l���g�̊Ǘ�
    Component* m_GameObject = this; // �I�u�W�F�N�g
};

/*
_Ty(const _Ty& obj);
_Ty &operator =(const _Ty & obj);
*/
