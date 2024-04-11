#pragma once
#include "./PlayerState.h"
#include "./../../../../../../DesignPatterns/FiniteStateMachine.h"

class Player::PlayerDefault : public Player::PlayerState
{
public:
	PlayerDefault(Player* owner);
	virtual ~PlayerDefault();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	�ړ��̓��͑��삪�������恙
	virtual void OnInputAttackDown() override;						//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() override;					//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;
	virtual void OnInputDash(bool input) override;
private:
	Player* m_owner;

	class State;
	class Stand;
	class Walk;
	class Run;
	enum class StateID
	{
		STAND,
		WALK,
		RUN,
	};
	Crown::FiniteStateMachine<StateID, State> m_state;

	static constexpr float WALK_ACCELERATION = 0.2f;
	static constexpr float WALK_DECELERATION = 0.985f;
	static constexpr float WALK_SPEED = 0.01f;

	static constexpr float RUN_SPEED = 0.1f;
};