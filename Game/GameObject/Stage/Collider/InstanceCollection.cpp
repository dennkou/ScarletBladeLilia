#include "InstanceCollection.h"
#include "PlayerCollider.h"
#include "PlayerAttackCollider.h"
#include "EnemyCollider.h"

ColliderSystem::InstanceCollection::InstanceCollection()
{

}

ColliderSystem::InstanceCollection::~InstanceCollection()
{

}

void ColliderSystem::InstanceCollection::Update()
{
	//	ƒvƒŒƒCƒ„[Œn™
	for (EnemyCollider* enemyCollider : m_enemyCollider)
	{
		m_playerCollider->CheckHitEnemy(enemyCollider);

		for (PlayerAttackCollider* playerAttackCollider : m_playerAttackCollider)
		{
			playerAttackCollider->CheckHitEnemy(enemyCollider);

		}
	}


}



void ColliderSystem::InstanceCollection::SetPlayerCollider(PlayerCollider* playerCollider)
{
	m_playerCollider = playerCollider;
}

void ColliderSystem::InstanceCollection::DeletePlayerCollider()
{
	m_playerCollider = nullptr;
}

ColliderSystem::PlayerCollider* ColliderSystem::InstanceCollection::GetPlayerCollider()
{
	return m_playerCollider;
}



void ColliderSystem::InstanceCollection::SetPlayerAttackCollider(PlayerAttackCollider* playerAttackCollider)
{
	m_playerAttackCollider.push_back(playerAttackCollider);
}

void ColliderSystem::InstanceCollection::DeletePlayerAttackCollider(PlayerAttackCollider* playerAttackCollider)
{
	m_playerAttackCollider.erase(std::find(m_playerAttackCollider.begin(), m_playerAttackCollider.end(), playerAttackCollider));
}

std::vector<ColliderSystem::PlayerAttackCollider*>* ColliderSystem::InstanceCollection::GetPlayerAttackColliders()
{
	return &m_playerAttackCollider;
}



void ColliderSystem::InstanceCollection::SetEnemyCollider(EnemyCollider* enemyCollider)
{
	m_enemyCollider.push_back(enemyCollider);
}

void ColliderSystem::InstanceCollection::DeleteEnemyCollider(EnemyCollider* enemyCollider)
{
	m_enemyCollider.erase(std::find(m_enemyCollider.begin(), m_enemyCollider.end(), enemyCollider));
}

std::vector<ColliderSystem::EnemyCollider*>* ColliderSystem::InstanceCollection::GetEnemyColliders()
{
	return &m_enemyCollider;
}



void ColliderSystem::InstanceCollection::SetStageCollider(StageCollider* stageCollider)
{
	m_stageCollider.push_back(stageCollider);
}

void ColliderSystem::InstanceCollection::DeleteStageCollider(StageCollider* stageCollider)
{
	m_stageCollider.erase(std::find(m_stageCollider.begin(), m_stageCollider.end(), stageCollider));
}

std::vector<ColliderSystem::StageCollider*>* ColliderSystem::InstanceCollection::GetStageColliders()
{
	return &m_stageCollider;
}
