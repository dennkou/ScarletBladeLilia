#pragma once
#include "ColliderSystem.h"

#include "PlayerCollider.h"
#include "EnemyCollider.h"

class ColliderSystem::StageCollider
{
public:
	StageCollider();
	~StageCollider();

	void CheckHitPlayer(PlayerCollider* playerCollider);
	void CheckHitEnemy(EnemyCollider* enemyCollider);
private:
};