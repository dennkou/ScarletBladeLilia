#pragma once
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include <functional>

//	�G�l�~�[�̋�Ԃɑ΂���֐S���܂Ƃ߂����̂��恙
class ColliderSystem::EnemyCollider
{
public:
	EnemyCollider(std::function<void(float)> hit, std::function<void(void)> wallHit);
	virtual ~EnemyCollider();

	void Hit(float damage);														//	�U�����󂯂��恙
	void HitWall();

	void SetPosition(DirectX::XMFLOAT3 position) noexcept;								//	�G�l�~�[�̈ʒu��ݒ肷��恙
	inline DirectX::XMFLOAT3 GetPosition() const noexcept { return m_sphere.point.point; }		//	�G�l�~�[�̈ʒu���擾����恙
	inline void SetRadius(float radius) noexcept { m_sphere.radius = radius; }					//	�����蔻��̃J�v�Z���̔��a��ݒ肷��恙
	inline ColliderAlgorithm::Sphere GetCollision() const noexcept { return m_sphere; }			//	�����蔻��̃J�v�Z�����擾����恙
	inline void SetPlayerPosition(DirectX::XMFLOAT3 pos) { m_playerPosition = pos; }			//	�v���C���[�ʒu����͂���恙
	inline DirectX::XMFLOAT3 GetPlayerPosition() const noexcept { return m_playerPosition; }	//	�v���C���[�̈ʒu���擾����恙
	inline bool IsEngagement() const noexcept { return m_engagement; }							//	�v���C���[�𔭌����Ă��邩�ǂ������恙
private:
	ColliderAlgorithm::Sphere m_sphere;
	std::vector<ColliderAlgorithm::Triangle> m_attack;
	std::function<void(float)> m_hit;
	std::function<void(void)> m_wallHit;
	DirectX::XMFLOAT3 m_playerPosition;
	bool m_engagement;
};