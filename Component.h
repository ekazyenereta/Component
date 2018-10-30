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

class GameObject; // �O��

//==========================================================================
//
// class  : Component
// Content: �R���|�[�l���g
//
//==========================================================================
class Component
{
public:
    /**
    @brief �R���|�[�l���g�̃R���X�g���N�^
    */
    Component() {
        m_GameObject = nullptr;
    }

    /**
    @brief �R���|�[�l���g�̃f�X�g���N�^
    */
    virtual ~Component() {
        // �j��
        Destroy(m_GameObject);
    }

    /**
    @brief �Q�[���I�u�W�F�N�g�̎擾
    @return �I�u�W�F�N�g
    */
    GameObject * GetGameObject() {
        // null �̏ꍇ�͎��̂𐶐������̂�Ԃ��A���Ԃ�����ꍇ�͎��̂�Ԃ�
        return m_GameObject == nullptr ? m_GameObject = Initializer<GameObject>() : m_GameObject;
    }

    /**
    @brief �R���|�[�l���g�̎擾
    @return �R���|�[�l���g
    */
    template <typename _Ty>
    _Ty* GetComponent() {
        auto *component = dynamic_cast<_Ty*>(this);
        return component != nullptr ? component : nullptr;
    }

    /**
    @brief �Q�[���I�u�W�F�N�g�̓o�^
    */
    void SetGameObject(GameObject * obj) {
        m_GameObject = obj;
    }
private:
    /**
    @brief �I�u�W�F�N�g�̔j��
    */
    template<typename _Ty>
    void Destroy(_Ty *& obj) {
        // �e���v���[�g�ɂ�郁�������
        if (obj != nullptr) {
            delete obj;
            obj = nullptr;
        }
    }

    /**
    @brief �I�u�W�F�N�g�̐���
    @return �I�u�W�F�N�g
    */
    template<typename _Ty>
    _Ty * Initializer() {
        // �e���v���[�g�ɂ�郁�����m��
        return new _Ty;
    }
private:
    GameObject* m_GameObject; // �I�u�W�F�N�g
};

//==========================================================================
//
// class  : GameObject
// Content: �Q�[���I�u�W�F�N�g
//
//==========================================================================
class GameObject
{
public:
    /**
    @brief �I�u�W�F�N�g�̃R���X�g���N�^
    */
    GameObject() {}

    /**
    @brief �I�u�W�F�N�g�̃f�X�g���N�^
    */
    virtual ~GameObject() {
        // �R���|�[�l���g�̔j��
        for (auto * itr : m_ComponentList) {
            delete itr;
            itr = nullptr;
        }
        m_ComponentList.clear();
    }

    /**
    @brief �Q�[���I�u�W�F�N�g�̎擾
    @return �I�u�W�F�N�g
    */
    GameObject * GetGameObject() {
        // null �̏ꍇ�͎��̂𐶐������̂�Ԃ��A���Ԃ�����ꍇ�͎��̂�Ԃ�
        return this;
    }

    /**
    @brief �R���|�[�l���g�̒ǉ��BComponent ���p�����Ă��Ȃ��ꍇ�G���[���o�܂��B
    @return �R���|�[�l���g
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* AddComponent() {
        // �R���|�[�l���g���p������Ă��Ȃ��ꍇ�ɏo�͂���܂��B
        static_assert(isExtended, "AddComponent<> : _Ty is not inherited from Component Class");
        auto *component = new _Ty();
        m_ComponentList.push_back(component);
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
        auto *component = new _Ty((_Val)...);
        m_ComponentList.push_back(component);
        return component;
    }

    /**
    @brief �R���|�[�l���g�̎擾
    @return �R���|�[�l���g
    */
    template <typename _Ty>
    _Ty* GetComponent() {
        // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��Ԃ�Ԃ�
        for (auto *& itr : m_ComponentList) {
            auto *component = dynamic_cast<_Ty*>(itr);
            if (component != nullptr) return component;
        }
        return nullptr;
    }

    /**
    @brief �q�v�f�̐���Ԃ��܂�
    @return �v�f��
    */
    auto GetNumChild() const {
        return m_ComponentList.size();
    }

    /**
    @brief �q�v�f��Ԃ��܂�
    @return �R���|�[�l���g�i�[�̈�
    */
    const auto GetChild() const {
        return m_ComponentList;
    }
private:
    std::list<Component*> m_ComponentList; // �R���|�[�l���g�̊Ǘ�
};
