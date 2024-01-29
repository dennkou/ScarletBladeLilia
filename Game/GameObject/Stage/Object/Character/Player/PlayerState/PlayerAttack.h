#pragma once
#include "./PlayerState.h"
#include <functional>
class Player::PlayerAttack : public Player::PlayerState
{
public:
	PlayerAttack(Player* player);
	virtual ~PlayerAttack();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;
private:
	void CameraAnim(float animFlame);
	void FlameProcess(float start, float end, std::function<void(float)> process);
	static constexpr float MOVING_DISTANCE = 1.43345f;	//	ˆÚ“®‹——£‚¾‚æ™
	static constexpr int MOVING_END_FLAME = 22;			//	ˆÚ“®‚ªI—¹‚·‚éƒtƒŒ[ƒ€‚¾‚æ™
	static constexpr float MAX_FOV_ANGLE = DirectX::XMConvertToRadians(90);
	static constexpr float DEFAULT_FOV_ANGLE = DirectX::XMConvertToRadians(60);
	static constexpr float MOVE_CANCEL_FLAME = 50;
	Player* m_player;
	DirectX::XMFLOAT3 m_startPosition;
};