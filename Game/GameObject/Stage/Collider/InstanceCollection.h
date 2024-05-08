#pragma once
#include "ColliderSystem.h"
#include <vector>

//	当たり判定のインスタンスを保持するだけのクラスだよ☆
class ColliderSystem::InstanceCollection
{
public:
	InstanceCollection();
	~InstanceCollection();

	void Update();

	void SetPlayerCollider(PlayerCollider* playerCollider);
	void DeletePlayerCollider();
	PlayerCollider* GetPlayerCollider();

	void SetPlayerAttackCollider(PlayerAttackCollider* playerAttackCollider);
	void DeletePlayerAttackCollider(PlayerAttackCollider* playerAttackCollider);
	std::vector<PlayerAttackCollider*>* GetPlayerAttackColliders();

	void SetEnemyCollider(EnemyCollider* enemyCollider);
	void DeleteEnemyCollider(EnemyCollider* enemyCollider);
	std::vector<EnemyCollider*>* GetEnemyColliders();

	void SetEnemyAttackCollider(EnemyAttackCollider* enemyAttackCollider);
	void DeleteEnemyAttackCollider(EnemyAttackCollider* enemyAttackCollider);
	std::vector<EnemyAttackCollider*>* GetEnemyAttackCollider();

	void SetStageCollider(StageCollider* stageCollider);
	void DeleteStageCollider(StageCollider* stageCollider);
	std::vector<StageCollider*>* GetStageColliders();



	void SetAisleCollider(AisleCollider* aisleCollider);
	void DeleteAisleCollider(AisleCollider* aisleCollider);
	std::vector<AisleCollider*>* GetAisleColliders();
private:

	PlayerCollider* m_playerCollider;
	std::vector<PlayerAttackCollider*> m_playerAttackCollider;
	std::vector<EnemyCollider*> m_enemyCollider;
	std::vector<EnemyAttackCollider*> m_enemyAttackCollider;
	std::vector<StageCollider*> m_stageCollider;

	std::vector<AisleCollider*> m_aisleCollider;
};