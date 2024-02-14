#pragma once
#include "./PlayerState.h"
#include <functional>
#include "./../../../../Collider/PlayerAttackCollider.h"

class Player::PlayerAttack : public Player::PlayerState
{
public:
	PlayerAttack(Player* player);
	virtual ~PlayerAttack();

	virtual void Enter() override;
	virtual void Update(float time) override;
	virtual void Exit() override;

	virtual void OnInputMove(DirectX::XMFLOAT2 input) override;
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) override;
private:
	void CameraAnim(float animFlame);
	void FlameProcess(float start, float end, std::function<void(float)> process);
	static constexpr float MOVING_DISTANCE = 1.43345f;	//	�ړ��������恙
	static constexpr int MOVING_END_FLAME = 22;			//	�ړ����I������t���[�����恙
	static constexpr float MAX_FOV_ANGLE = DirectX::XMConvertToRadians(90);
	static constexpr float DEFAULT_FOV_ANGLE = DirectX::XMConvertToRadians(60);
	static constexpr float MOVE_CANCEL_FLAME = 50;
	static constexpr float ATTACK_START_FLAME = 13;
	static constexpr float ATTACK_END_FLAME = 17;

	ColliderSystem::PlayerAttackCollider m_attackCollider;
	bool m_attackFlag;									//	�U������𔭐����������̃t���O���恙

	Player* m_player;
	DirectX::XMFLOAT3 m_startPosition;
};