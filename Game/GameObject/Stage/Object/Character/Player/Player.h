#pragma once
#include "./../Character.h"
#include "./PlayerCamera.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"
#include "./../../../Collider/PlayerCollider.h"

class Player : public Character
{
public:
	Player(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate);
	virtual ~Player() override;

	virtual void OnGameUpdate(Timer& timer) override;
	virtual void OnPlayStartAnimation() override;
	virtual void OnPlayStart() override;

	//	���͌n��
	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	�ړ��̓��͂��ς�����恙
	virtual void OnInputAttackDown() override;						//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() override;					//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;	//	
private:
	void CreateMaterial();
	void StateSetUp();
	void AnimLoad();
	void ColliderUpdate();

	//	���͂ɂ��J��������]������恙
	void CameraRoll(DirectX::XMFLOAT2 input);

	//	�v���C���[�̃X�e�[�^�X���恙
	static constexpr int MAX_HP = 1000;								//	�v���C���[�̍ő�HP���恙

	PlayerCamera m_camera;
	ColliderSystem::PlayerCollider m_collider;
	DirectX::XMMATRIX m_bone[255];									//	���݃v���C���[�����ׂ��|�[�Y���恙
	float m_stateTimer;												//	�X�e�[�g�ŗ��p����z��̃^�C�}�[���恙���̒l�������������͌��݂̃X�e�[�g�Ɉˑ�����恙
	DirectX::XMFLOAT2 m_inputMove;
	float m_targetAngle;											//	�v���C���[�������Ƃ��Ă���p�x���恙
	bool hasSword;													//	������Ɏ����Ă���恙

	class PlayerState;
	class PlayerTitle;
	class PlayerDefault;
	class PlayerAttack;
	class PlayerAvoidance;

	enum class StateID
	{
		Title,
		Normal,
		Attack,
		Avoidance,
	};

	Crown::FiniteStateMachine<StateID, PlayerState> m_playerState;	//	��Ԃ������X�e�[�g�}�V�����恙

	//	�A�j���[�V�����p�̒萔���ϐ����恙

	static constexpr float ANIMATION_FPS = 30;						//	�A�j���[�V�����̃t���[�����[�g���恙
	Crown::RenderObject::AnimationData m_standAnim;					
	static constexpr float STAND_ANIM_SPEED = 1.0f;					
	Crown::RenderObject::AnimationData m_runStartAnim;				
	static constexpr float RUN_START_ANIM_SPEED = 2.0f;				
	Crown::RenderObject::AnimationData m_runAnim;					
	static constexpr float RUN_ANIM_SPEED = 2.0f;					
	Crown::RenderObject::AnimationData m_drawingSwordAttackAnim;	
	static constexpr float DRAWING_SWORD_ATTACK_ANIM_SPEED = 2.0f;	

};