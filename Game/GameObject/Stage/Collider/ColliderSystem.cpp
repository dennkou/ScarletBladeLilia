#include "ColliderSystem.h"

ColliderSystem::ColliderSystem(Game* game)
	:
	GameObject(game),
	m_playerCollider(nullptr)
{

}

ColliderSystem::~ColliderSystem()
{

}

void ColliderSystem::OnGameUpdate(Timer& timer)
{
	timer;
}

void ColliderSystem::SetPlayerCollider(PlayerCollider* playerCollider)
{
	m_playerCollider = playerCollider;
}

void ColliderSystem::DeletePlayerCollider()
{
	m_playerCollider = nullptr;
}

ColliderSystem::PlayerCollider* ColliderSystem::GetPlayerCollider()
{
    return m_playerCollider;
}

void ColliderSystem::SetEnemyCollider(EnemyCollider* enemyCollider)
{
	m_enemyCollider.push_back(enemyCollider);
}

void ColliderSystem::DeleteEnemyCollider(EnemyCollider* enemyCollider)
{
	m_enemyCollider.erase(std::find(m_enemyCollider.begin(), m_enemyCollider.end(), enemyCollider));
}

std::vector<ColliderSystem::EnemyCollider*>* ColliderSystem::GetEnemyColliders()
{
	return &m_enemyCollider;
}

void ColliderSystem::SetStageCollider(StageCollider* stageCollider)
{
	m_stageCollider.push_back(stageCollider);
}

void ColliderSystem::DeleteStageCollider(StageCollider* stageCollider)
{
	m_stageCollider.erase(std::find(m_stageCollider.begin(), m_stageCollider.end(), stageCollider));
}

std::vector<ColliderSystem::StageCollider*>* ColliderSystem::GetStageColliders()
{
	return &m_stageCollider;
}
