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
// �O��
//==========================================================================
class Component;

//==========================================================================
//
// class  : GameObject
// Content: �Q�[���I�u�W�F�N�g
//
//==========================================================================
class GameObject
{
private:
    // �R�s�[�֎~ (C++11)
    GameObject(const GameObject &) = delete;
    GameObject &operator=(const GameObject &) = delete;
public:
    GameObject() {}
    virtual ~GameObject() {
        // �R���|�[�l���g�̔j��
        for (auto *& itr : m_child) {
            Destroy(itr);
        }
        m_child.clear();
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
        component->SetGameObject(this);
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
        component->SetGameObject(this);
        m_child.push_back(component);
        return component;
    }

    /**
    @brief �R���|�[�l���g�̎擾
    @return �R���|�[�l���g
    */
    template <typename _Ty, bool isExtended = std::is_base_of<Component, _Ty>::value>
    _Ty* GetComponent() {
        if (!isExtended)return nullptr;

        // �Ώۂ̃R���|�[�l���g���o������܂ő����A�o�������ꍇ�͂��̎��Ԃ�Ԃ�
        for (auto *& itr : m_child) {
            _Ty *component = dynamic_cast<_Ty*>(itr);
            if (component != nullptr) return component;
        }
        return dynamic_cast<_Ty*>(this);
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
private:
    std::list<Component*> m_child; // �R���|�[�l���g�̊Ǘ�
};

//==========================================================================
//
// class  : Component
// Content: �R���|�[�l���g
//
//==========================================================================
class Component : public GameObject
{
private:
    // �R�s�[�֎~ (C++11)
    Component(const Component &) = delete;
    Component &operator=(const Component &) = delete;
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
        m_GameObject = nullptr;
    }

    /**
    @brief �Q�[���I�u�W�F�N�g�̎擾
    @return �I�u�W�F�N�g
    */
    GameObject * GetGameObject() {
        return m_GameObject;
    }

    /**
    @brief �Q�[���I�u�W�F�N�g�̓o�^
    */
    void SetGameObject(GameObject * obj) {
        m_GameObject = obj;
    }
private:
    GameObject* m_GameObject = nullptr; // �I�u�W�F�N�g
};
