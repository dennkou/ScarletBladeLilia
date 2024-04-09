#pragma once
#include "ColliderSystem.h"
#include "ColliderAlgorithm.h"
#include <functional>

//	エネミーの空間に対する関心をまとめたものだよ☆
class ColliderSystem::EnemyCollider
{
public:
	EnemyCollider(std::function<void(int)> hit);
	virtual ~EnemyCollider();

	void Hit(int damage);																//	攻撃を受けたよ☆

	void SetPosition(DirectX::XMFLOAT3 position) noexcept;								//	エネミーの位置を設定するよ☆
	void SetCapsuleVector(DirectX::XMFLOAT3 capsuleVector) noexcept;					//	エネミーの体勢を設定するよ☆
	DirectX::XMFLOAT3 GetPosition() { return m_capsule.line.start.point; }				//	エネミーの位置を取得するよ☆
	void SetRadius(float radius) noexcept { m_capsule.radius = radius; }				//	当たり判定のカプセルの半径を設定するよ☆
	ColliderAlgorithm::Capsule GetCollision() const noexcept { return m_capsule; }		//	当たり判定のカプセルを取得するよ☆
	void SetPlayerPosition(DirectX::XMFLOAT3 pos) { m_playerPosition = pos; }			//	プレイヤー位置を入力するよ☆
	DirectX::XMFLOAT3 GetPlayerPosition() { return m_playerPosition; }					//	プレイヤーの位置を取得するよ☆
	bool IsEngagement() { return m_engagement; }										//	プレイヤーを発見しているかどうかだよ☆
private:
	ColliderAlgorithm::Capsule m_capsule;
	DirectX::XMFLOAT3 m_capsuleVector;
	std::function<void(int)> m_hit;
	DirectX::XMFLOAT3 m_playerPosition;
	bool m_engagement;
};