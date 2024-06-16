#include "Shot.h"
#include "./../../../../Bullet.h"

Enemy::EnemyAIStateCombat::Shot::Shot(EnemyAIStateCombat* owner)
	:
	m_owner(owner),
	m_shotFlag(false),
	m_timer(0)
{
}

Enemy::EnemyAIStateCombat::Shot::~Shot()
{
}

void Enemy::EnemyAIStateCombat::Shot::Enter()
{
	m_shotFlag = false;
	m_timer = 0;
}

void Enemy::EnemyAIStateCombat::Shot::Update(float time)
{
	Enemy* enemy = m_owner->m_enemy;

	m_timer += time;

	if ((!m_shotFlag) && time > SHOT_FLAME)
	{
		m_shotFlag = true;
		DirectX::XMVECTOR positionVector;
		DirectX::XMVECTOR rotateVector;
		DirectX::XMVECTOR scaleVector;
		DirectX::XMMATRIX mat = enemy->m_model.GetBoneDate().Find(L"‘S‚Ä‚Ìe").GetWorldMatrix(enemy->m_bone);
		DirectX::XMMatrixDecompose(&positionVector, &rotateVector, &scaleVector, mat);
		DirectX::XMFLOAT3 rotate;
		DirectX::XMStoreFloat3(&rotate, rotateVector);
		DirectX::XMFLOAT3 position;
		DirectX::XMStoreFloat3(&position, positionVector);
		DirectX::XMFLOAT3 vector;
		vector.x = cos(rotate.x) * sin(rotate.y);
		vector.y = sin(rotate.x);
		vector.z = cos(rotate.x) * cos(rotate.y);
		enemy->CreateGameObject<Bullet>(position, vector);
	}

	enemy->m_shotAnim.GetAnimation(m_timer / ANIMATION_FPS, enemy->m_bone, enemy->m_model.GetBoneDate());

	if (m_timer > enemy->m_shotAnim.GetMaxFrame())
	{
		m_owner->ActionBranch();
	}
}

void Enemy::EnemyAIStateCombat::Shot::Exit()
{
}
