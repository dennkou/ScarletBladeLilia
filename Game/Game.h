#pragma once
//	�f�o�b�N���[�h�̐ݒ肾�恙
#ifdef _DEBUG

//	���O�\���n��
//#define DISPLAY_LOG		//	���O��\������恙
#ifdef DISPLAY_LOG
#include <unordered_map>
#define IGNORE_UPDATE_EVENT	//	�A�b�v�f�[�g�C�x���g�̃��O��\�����Ȃ��恙
#endif // DISPLAY_LOG



#endif // _DEBUG

#include <vector>
#include <memory>
#include "./DesignPatterns/FiniteStateMachine.h"
#include <iostream>
#include <chrono>
#include <mutex>




#include "./Timer.h"



#ifndef GAME
#define GAME
/// <summary>
/// <para>���̃N���X��GameObject���p�������N���X��o�^���ăQ�[����g�ݗ��Ă����恙</para>
/// <para>�e�Q�[���I�u�W�F�N�g�̓C�x���g����ĘA�g���Ă����恙</para>
/// </summary>
class Game
{
public:
	// �Q�[���I�u�W�F�N�g�̈ʒu��\���^���恙
	using ObjectIndex = unsigned int;

	// �Q�[�����Ŏg�p����N���X��
	class GameObject;
	Game();
	~Game();

	// �X�V�̎��s��
	void Update();

	//	�Q�[�����I�������ʂ��������t���O��Ԃ��恙
	inline bool GetGameEndFlag() const noexcept { return m_endFlag; }

private:
	
	void GameEnd();

	// �C�x���g�����s����恙
	template<class... Argument>
	void EventTrigger(void(GameObject::*handler)(Argument...), Argument... argument)
	{
		//	���݂��Ȃ��I�u�W�F�N�g���폜���Ȃ���S�ẴI�u�W�F�N�g�̃C�x���g���ĂԂ恙

		unsigned int moveNum = 0;	//	���݂��ĂȂ������I�u�W�F�N�g�̐����恙

		ObjectIndex i = 0;

		//	�w�肳�ꂽ�֐��̌Ăяo�����@���s�ɂ��T�C�Y���ύX���邩���m��Ȃ�����size�֐����g�p����恙
		static const ObjectIndex objectNum = static_cast<ObjectIndex>(m_gameObjects.size());
		while (i + moveNum < objectNum)
		{
			m_gameObjects[i] = m_gameObjects[i + moveNum];

			//	�I�u�W�F�N�g�����݂��邩�m�F��
			if (m_gameObjects[i])
			{
				(m_gameObjects[i]->*handler)(argument...);
				++i;
			}
			else
			{
				++moveNum;
			}
		}

		m_gameObjects.erase(m_gameObjects.end() - moveNum, m_gameObjects.end());	//	�폜�������v�f�����炷�恙
	}

	//	�Q�[���I�u�W�F�N�g��ǉ�����恙
	template<class Type, class... Argument>
	GameObject* CreateGameObject(Argument... argument)
	{
		GameObject* ret = new Type(this,argument...);
		m_gameObjects.emplace_back(ret);

		#ifdef DISPLAY_LOG
		{
			m_gameObjectName[ret] = typeid(Type).name();
			std::cout << m_gameObjectName[ret] << "�N���X�𐶐������恙" << std::endl;

		}
		#endif // DISPLAY_LOG

		return ret;
	}

	//	�Q�[���I�u�W�F�N�g���폜����恙
	void DeleteGameObject(GameObject* gameObject);



	std::vector<GameObject*> m_gameObjects;							//	�Q�[���I�u�W�F�N�g�̔z�񂾂恙

	#ifdef DISPLAY_LOG
		std::unordered_map<GameObject*, std::string> m_gameObjectName;	//	�Q�[���I�u�W�F�N�g���̔z�񂾂恙

		double m_updateTime;
		unsigned long long m_updateNum;
	#endif // DISPLAY_LOG
	bool m_endFlag;

	Timer m_timer;
};
#endif GAME