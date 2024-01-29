#pragma once
#include "./../../GameObject.h"
#include <vector>

class ColliderSystem : public Game::GameObject
{
public:
	class PlayerCollider;
	class EnemyCollider;
	class StageCollider;

	ColliderSystem(Game* game);
	virtual ~ColliderSystem();

	virtual void OnGameUpdate(Timer& timer) override;
private:
	void SetPlayerCollider(PlayerCollider* playerCollider);
	void DeletePlayerCollider();
	PlayerCollider* GetPlayerCollider();
	void SetEnemyCollider(EnemyCollider* enemyCollider);
	void DeleteEnemyCollider(EnemyCollider* enemyCollider);
	std::vector<EnemyCollider*>* GetEnemyColliders();
	void SetStageCollider(StageCollider* stageCollider);
	void DeleteStageCollider(StageCollider* stageCollider);
	std::vector<StageCollider*>* GetStageColliders();



	PlayerCollider* m_playerCollider;
	std::vector<EnemyCollider*> m_enemyCollider;
	std::vector<StageCollider*> m_stageCollider;
};