#pragma once
#include "ColliderSystem.h"
#include <vector>

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

	void SetStageCollider(StageCollider* stageCollider);
	void DeleteStageCollider(StageCollider* stageCollider);
	std::vector<StageCollider*>* GetStageColliders();
private:

	PlayerCollider* m_playerCollider;
	std::vector<PlayerAttackCollider*> m_playerAttackCollider;
	std::vector<EnemyCollider*> m_enemyCollider;
	std::vector<StageCollider*> m_stageCollider;
};