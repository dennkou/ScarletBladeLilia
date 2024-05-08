#pragma once
#include "./../../Game/GameObject/GameObject.h"
#include "./PlayerCamera.h"
#include "./../../../../../DesignPatterns/FiniteStateMachine.h"
#include "./../../Crown/Object/RenderSystem/Animation/AnimationData.h"
#include "./../../Crown/Object/RenderSystem/Model/Model.h"
#include "./../../../Collider/PlayerCollider.h"
#include "./UI/PlayerHpUi.h"
#include "./../HP.h"

class Player : public Game::GameObject
{
public:
	//	�v���C���[�����J����p�����[�^���恙
	static constexpr DirectX::XMFLOAT3 HEAD_OFFSET = DirectX::XMFLOAT3(0,15,0);

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
	virtual void OnInputDash(bool input) override;
private:
	void CreateMaterial();
	void StateSetUp();
	void AnimLoad();
	void ColliderUpdate();

	//	���͂ɂ��J��������]������恙
	void CameraRoll(DirectX::XMFLOAT2 input);

	void Hit(float damage);

	static constexpr float LONG_PRESS_TIME = 1.0;

	//	�v���C���[�̃X�e�[�^�X���恙
	static constexpr int MAX_HP = 1000;								//	�v���C���[�̍ő�HP���恙

	static constexpr float WALK_ACCELERATION = 0.2f;
	static constexpr float WALK_DECELERATION = 0.985f;
	static constexpr float WALK_SPEED = 0.01f;

	static constexpr float RUN_SPEED = 0.1f;

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotate;

	PlayerCamera m_camera;
	ColliderSystem::PlayerCollider m_collider;
	HP m_hp;
	Crown::RenderObject::Model m_model;
	PlayerHpUi m_hpUi;
	DirectX::XMMATRIX m_bone[255];									//	���݃v���C���[�����ׂ��|�[�Y���恙
	DirectX::XMFLOAT2 m_inputMove;
	DirectX::XMFLOAT3 m_velocity;

	class PlayerState;
	class PlayerTitle;
	class PlayerStand;
	class PlayerWalk;
	class PlayerRun;
	class PlayerAttack;
	class PlayerAvoidance;

	enum class StateID
	{
		Title,
		Stand,
		Walk,
		Run,
		Attack,
		Avoidance,
	};

	Crown::FiniteStateMachine<StateID, PlayerState> m_playerState;	//	��Ԃ������X�e�[�g�}�V�����恙

	//	�A�j���[�V�����p�̒萔���ϐ����恙

	static constexpr float ANIMATION_FPS = 30;						//	�A�j���[�V�����̃t���[�����[�g���恙

	Crown::RenderObject::AnimationData m_standAnim;					
	static constexpr float STAND_ANIM_SPEED = 1.0f;					

	Crown::RenderObject::AnimationData m_walkStartAnim;
	static constexpr float WALK_START_ANIM_SPEED = 2.0f;
	Crown::RenderObject::AnimationData m_walkAnim;
	static constexpr float WALK_ANIM_SPEED = 1.0f;

	Crown::RenderObject::AnimationData m_runStartAnim;				
	static constexpr float RUN_START_ANIM_SPEED = 2.0f;				
	Crown::RenderObject::AnimationData m_runAnim;					
	static constexpr float RUN_ANIM_SPEED = 7.0f;					

	Crown::RenderObject::AnimationData m_frontStepAnim;
	static constexpr float FRONT_STEP_ANIM_SPEED = 1.0f;

	Crown::RenderObject::AnimationData m_drawingSwordAttackAnim;	
	static constexpr float DRAWING_SWORD_ATTACK_ANIM_SPEED = 2.0f;	

};