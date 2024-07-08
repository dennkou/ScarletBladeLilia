#pragma once
#include "PlayerState.h"
#include "./../../../../Collider/PlayerAttackCollider.h"

class Player::PlayerSlashAttack : public Player::PlayerState
{
public:
	PlayerSlashAttack(Player* player);
	virtual ~PlayerSlashAttack();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	//	���͌n��
	virtual void OnInputMove(DirectX::XMFLOAT2 input) { input; }	//	�ړ��̓��͑��삪�������恙
	virtual void OnInputAttackDown() {}								//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() {}							//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) { input; }	//	�J�����̓��͂��������恙
	virtual void OnInputDash(bool input) { input; }
	virtual void OnPlayStartAnimation() {};							//	�v���C�J�n���I�����ꂽ�恙
	virtual void OnPlayStart() {};									//	�Q�[���v���C���J�n���ꂽ�恙
private:
	static constexpr float ROTATE_START_FLAME = 0;								//	
	static constexpr float ROTATE_END_FLAME = 5;								//	
	static constexpr float ROTATE_SPEED = DirectX::XMConvertToRadians(10.0f);	//	
	static constexpr float ATTACK_START_FLAME = 10;								//	
	static constexpr float ATTACK_END_FLAME = 13;								//	
	static constexpr float MOVING_DISTANCE = 6.0f;								//	
	static constexpr float MOVE_END = 60.0f;									//	
	static constexpr float END = 70.0f;											//	

	Player* m_player;
	float m_animTimer;

	ColliderSystem::PlayerAttackCollider m_attackCollider;
};