#pragma once
#include "./../PlayerState.h"
#include "./../../../../../../../DesignPatterns/FiniteStateMachine.h"

class Player::PlayerDefault : public Player::PlayerState
{
public:
	PlayerDefault(Player* owner);
	virtual ~PlayerDefault();

	virtual void Enter() override;
	virtual void Exit() override;
	virtual void Update(float time) override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;		//	移動の入力操作があったよ☆
	virtual void OnInputAttackDown() override;						//	攻撃ボタンが押されたよ☆
	virtual void OnInputAvoidanceDown() override;					//	回避ボタンが押されたよ☆
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;
private:
	void MoveAngleUpdate();
	Player* m_owner;

	float m_speed;

	class PlayerDefaultStateBase;
	class PlayerStand;
	class PlayerRun;

	enum class State
	{
		Stand,
		Run
	};

	State m_nowState;
	Crown::FiniteStateMachine<State, PlayerDefaultStateBase> m_state;
};