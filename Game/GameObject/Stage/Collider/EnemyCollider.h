#pragma once
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include <functional>

//	エネミーの空間に対する関心をまとめたものだよ☆
class ColliderSystem::EnemyCollider
{
public:
	EnemyCollider(std::function<void(float)> hit, std::function<void(void)> wallHit);
	virtual ~EnemyCollider();

	void Hit(float damage);														//	攻撃を受けたよ☆
	void HitWall();

	void SetPosition(DirectX::XMFLOAT3 position) noexcept;								//	エネミーの位置を設定するよ☆
	inline DirectX::XMFLOAT3 GetPosition() const noexcept { return m_sphere.point.point; }		//	エネミーの位置を取得するよ☆
	inline void SetRadius(float radius) noexcept { m_sphere.radius = radius; }					//	当たり判定のカプセルの半径を設定するよ☆
	inline ColliderAlgorithm::Sphere GetCollision() const noexcept { return m_sphere; }			//	当たり判定のカプセルを取得するよ☆
	inline void SetPlayerPosition(DirectX::XMFLOAT3 pos) { m_playerPosition = pos; }			//	プレイヤー位置を入力するよ☆
	inline DirectX::XMFLOAT3 GetPlayerPosition() const noexcept { return m_playerPosition; }	//	プレイヤーの位置を取得するよ☆
	inline bool IsEngagement() const noexcept { return m_engagement; }							//	プレイヤーを発見しているかどうかだよ☆
private:
	ColliderAlgorithm::Sphere m_sphere;
	std::vector<ColliderAlgorithm::Triangle> m_attack;
	std::function<void(float)> m_hit;
	std::function<void(void)> m_wallHit;
	DirectX::XMFLOAT3 m_playerPosition;
	bool m_engagement;
};