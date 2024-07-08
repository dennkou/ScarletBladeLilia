#pragma once
#include "PlayerState.h"

class Player::PlayerWalk : public Player::PlayerState
{
public:
	PlayerWalk(Player* player);
	virtual ~PlayerWalk();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	�ړ��̓��͑��삪�������恙
	virtual void OnInputDash(bool input) override;
	virtual void OnInputAttackDown() override;						//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() override;					//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;	//	�J�����̓��͂��������恙
private:
	enum class State
	{
		Start,
		Continuation,
		End,
		Turn,
	};

	static constexpr float ROLL_SPEED = 0.005f;
	Player* m_player;
	DirectX::XMFLOAT2 m_inputVector;
	float m_time;
	State m_state;
};