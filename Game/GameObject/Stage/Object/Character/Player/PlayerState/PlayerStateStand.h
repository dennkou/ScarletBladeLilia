#pragma once
#include "PlayerState.h"

class Player::PlayerStand : public Player::PlayerState
{
public:
	PlayerStand(Player* player);
	virtual ~PlayerStand();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	�ړ��̓��͑��삪�������恙
	virtual void OnInputDash(bool input) override;
	virtual void OnInputAttackDown() override;						//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() override;					//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;	//	�J�����̓��͂��������恙
private:
	Player* m_player;
	float m_animTimer;
};