#pragma once
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include <functional>

//	�G�l�~�[�̋�Ԃɑ΂���֐S���܂Ƃ߂����̂��恙
class ColliderSystem::EnemyCollider
{
public:
	EnemyCollider(std::function<void(int)> hit);
	virtual ~EnemyCollider();

	void Hit(int damage);																//	�U�����󂯂��恙

	void SetPosition(DirectX::XMFLOAT3 position) noexcept;								//	�G�l�~�[�̈ʒu��ݒ肷��恙
	void SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector) noexcept;					//	�G�l�~�[�̑̐���ݒ肷��恙
	DirectX::XMFLOAT3 GetPosition() { return m_capsule.line.start.point; }				//	�G�l�~�[�̈ʒu���擾����恙
	void SetRadius(float radius) noexcept { m_capsule.radius = radius; }				//	�����蔻��̃J�v�Z���̔��a��ݒ肷��恙
	ColliderAlgorithm::Capsule GetCollision() const noexcept { return m_capsule; }		//	�����蔻��̃J�v�Z�����擾����恙
	void SetPlayerPosition(DirectX::XMFLOAT3 pos) { m_playerPosition = pos; }			//	�v���C���[�ʒu����͂���恙
	DirectX::XMFLOAT3 GetPlayerPosition() { return m_playerPosition; }					//	�v���C���[�̈ʒu���擾����恙
	bool IsEngagement() { return m_engagement; }										//	�v���C���[�𔭌����Ă��邩�ǂ������恙
private:
	ColliderAlgorithm::Capsule m_capsule;
	DirectX::XMFLOAT3 m_capsuleVector;
	std::function<void(int)> m_hit;
	DirectX::XMFLOAT3 m_playerPosition;
	bool m_engagement;
};