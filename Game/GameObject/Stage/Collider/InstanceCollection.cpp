#include "InstanceCollection.h"
#include "PlayerCollider.h"
#include "PlayerAttackCollider.h"
#include "EnemyCollider.h"
#include "EnemyAttackCollider.h"
#include "AisleCollider.h"

ColliderSystem::InstanceCollection::InstanceCollection()
{

}

ColliderSystem::InstanceCollection::~InstanceCollection()
{

}

void ColliderSystem::InstanceCollection::Update()
{
	//	プレイヤー系☆
	for (EnemyCollider* enemyCollider : m_enemyCollider)
	{
		m_playerCollider->CheckHitEnemy(enemyCollider);
		enemyCollider->SetPlayerPosition(m_playerCollider->GetPosition());

		for (PlayerAttackCollider* playerAttackCollider : m_playerAttackCollider)
		{
			playerAttackCollider->CheckHitEnemy(enemyCollider);
		}
	}
	for (EnemyAttackCollider* enemyAttackCollider : m_enemyAttackCollider)
	{
		enemyAttackCollider->CheckHitPlayer(m_playerCollider);
	}

	//	ステージの判定☆
	for (AisleCollider* aisleCollider : m_aisleCollider)
	{
		aisleCollider->CheckHitPlayer(m_playerCollider);

		for (EnemyCollider* enemy : m_enemyCollider)
		{
			aisleCollider->CheckHitEnemy(enemy);
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

void ColliderSystem::InstanceCollection::SetEnemyAttackCollider(EnemyAttackCollider* enemyAttackCollider)
{
	m_enemyAttackCollider.push_back(enemyAttackCollider);
}

void ColliderSystem::InstanceCollection::DeleteEnemyAttackCollider(EnemyAttackCollider* enemyAttackCollider)
{
	m_enemyAttackCollider.erase(std::find(m_enemyAttackCollider.begin(), m_enemyAttackCollider.end(), enemyAttackCollider));
}

std::vector<ColliderSystem::EnemyAttackCollider*>* ColliderSystem::InstanceCollection::GetEnemyAttackCollider()
{
	return &m_enemyAttackCollider;
}

void ColliderSystem::InstanceCollection::SetBulletCollider(BulletCollider* bulletCollider)
{
	m_bulletCollider.push_back(bulletCollider);
}

void ColliderSystem::InstanceCollection::DeleteBulletCollider(BulletCollider* bulletCollider)
{
	m_bulletCollider.erase(std::find(m_bulletCollider.begin(), m_bulletCollider.end(), bulletCollider));
}

std::vector<ColliderSystem::BulletCollider*>* ColliderSystem::InstanceCollection::GetBulletCollider()
{
	return &m_bulletCollider;
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





void ColliderSystem::InstanceCollection::SetAisleCollider(AisleCollider* aisleCollider)
{
	m_aisleCollider.push_back(aisleCollider);
}

void ColliderSystem::InstanceCollection::DeleteAisleCollider(AisleCollider* aisleCollider)
{
	m_aisleCollider.erase(std::find(m_aisleCollider.begin(), m_aisleCollider.end(), aisleCollider));
}

std::vector<ColliderSystem::AisleCollider*>* ColliderSystem::InstanceCollection::GetAisleColliders()
{
	return &m_aisleCollider;
}
