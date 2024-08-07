#include "Enemy.h"
#include "AIState/EnemyAIState.h"
#include "AIState/EnemyAIStateTitle.h"
#include "AIState/EnemyAIStatePatrol.h"
#include "AIState/EnemyAIStateCombat.h"
#include "./../../../../Render/MaterialFactory.h"

#include "./../../../../../../MathLibrary.h"

Enemy::Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate, NavigationAI* navigationAI, std::shared_ptr<std::mt19937> random)
	:
	GameObject(game),
	START_POSITION(positon),
	START_ROTATE(rotate),
	m_position(positon),
	m_rotate(rotate),
	m_hp(MAX_HP),
	m_enemyCollider([&](float damage) { HitPlayerAttack(damage); }, [&]() { HitWall(); }),
	m_offset(),
	m_random(random),
	m_damageAnimTimer(DamageAnimLength),
	m_damageAnim(),
	m_isActive(true)
{
	m_model.LoadPMX(L"Resource/Model/PMX/Enemy.pmx");

	m_enemyCollider.SetRadius(8.0f);
	m_enemyCollider.SetPosition(positon);

	m_aiState.RegisterState<EnemyAIStateTitle>(AIState::Title, this);
	m_aiState.RegisterState<EnemyAIStatePatrol>(AIState::Patrol,this, navigationAI);
	m_aiState.RegisterState<EnemyAIStateCombat>(AIState::Combat,this);

	m_aiState.ChangeState(AIState::Title);

	m_ui.SetPositionOffset(UI_POSITION_OFFSET);
	m_ui.SetPosition(positon);
	m_ui.SetHPPercent(1);

	m_standAnim.LaodVMD(L"Resource/Motion/EnemyStand.vmd");
	m_attackAnim.LaodVMD(L"Resource/Motion/EnemyAttack.vmd");
	m_attack1Anim.LaodVMD(L"Resource/Motion/EnemyAttack1.vmd");
	m_shotAnim.LaodVMD(L"Resource/Motion/EnemyShot.vmd");

	for (int i = 0; i < 255; i++)
	{
		m_bone[i] = DirectX::XMMatrixIdentity();
	}
	MaterialSetup();
}

Enemy::~Enemy()
{

}

void Enemy::OnGameUpdate(Timer& timer)
{
	if (!m_isActive)
	{
		return;
	}

	if (m_damageAnimTimer < DamageAnimLength)
	{
		m_damageAnimTimer += timer.GetEnemyTime() / 1000;

		std::uniform_real_distribution<float> dist1(-1.0f, 1.0f);

		m_damageAnim = DirectX::XMFLOAT3(dist1(*m_random.get()), dist1(*m_random.get()), dist1(*m_random.get()));
	}
	else
	{
		m_damageAnim = DirectX::XMFLOAT3(0, 0, 0);
	}

	m_position = Crown::Math::VectorSub(m_enemyCollider.GetPosition(), m_offset);

	m_aiState.CallStateFunction(&EnemyAIState::Update, timer.GetEnemyTime());

	if (m_position.y > 0)
	{
		m_position.y -= 0.1f * timer.GetEnemyTime();
	}

	m_model.SetPause(m_bone);

	//	当たり判定の位置を設定するよ☆
	m_model.GetBoneDate().Find(L"全ての親").RecursiveMatrixMultiply(m_bone);
	unsigned int index = m_model.GetBoneDate().Find(L"本体").GetIndex();
	m_offset = Crown::Math::VectorAdd(DirectX::XMFLOAT3(m_bone[index].r[3].m128_f32[0], m_bone[index].r[3].m128_f32[1], m_bone[index].r[3].m128_f32[2]), BONE_OFFSET);

	m_enemyCollider.SetPosition(Crown::Math::VectorAdd(m_offset, m_position));
	m_ui.SetPosition(m_position);
	m_model.SetPosition(Crown::Math::VectorAdd(m_position, m_damageAnim));
	m_model.SetRotate(m_rotate);
}

void Enemy::HitPlayerAttack(float damage)
{
	if (!m_isActive)
	{
		return;
	}

	m_damageAnimTimer = 0.0f;
	m_hp.Damage(damage);
	m_aiState.CallStateFunction(&EnemyAIState::OnDamage, damage);
	m_ui.SetHPPercent(m_hp.GetHPPercent());

	//	死亡処理だよ☆
	if (m_hp.IsDied())
	{
		EventTrigger(&GameObject::OnEnemyDied, this);

		m_model.SetDrawFlag(false);
		m_ui.SetDrawFlag(false);
		m_enemyCollider.SetActive(false);
		m_isActive = false;
	}
}

void Enemy::OnPlayRestart()
{
	m_offset = DirectX::XMFLOAT3(0, 0, 0);
	m_position = START_POSITION;
	m_rotate = START_ROTATE;
	m_model.SetDrawFlag(true);
	m_ui.SetDrawFlag(true);
	m_enemyCollider.SetActive(true);
	m_isActive = true;
	m_enemyCollider.SetPosition(m_position);
	m_ui.SetPosition(m_position);
	m_model.SetPosition(m_position);
	m_model.SetRotate(m_rotate);
	m_aiState.ChangeState(AIState::Patrol);
}

void Enemy::HitWall()
{
	if (!m_isActive)
	{
		return;
	}
	m_aiState.CallStateFunction(&EnemyAIState::OnWallHit);
}

void Enemy::MaterialSetup()
{
	MaterialFactory::CreateShadow(m_model, 0);
	MaterialFactory::CreateShadow(m_model, 1);
	MaterialFactory::CreateShadow(m_model, 2);
	MaterialFactory::CreateShadow(m_model, 3);
	MaterialFactory::CreateDefaultMaterial(m_model, 0, 0, DirectX::XMFLOAT4(0.3f, 0, 0, 1), DirectX::XMFLOAT3(0,0,0));
	MaterialFactory::CreateDefaultMaterial(m_model, 1, 0, DirectX::XMFLOAT4(0, 1, 1, 1), DirectX::XMFLOAT3(0,0,0));
	MaterialFactory::CreateDefaultMaterial(m_model, 2, 0, DirectX::XMFLOAT4(0, 0, 0, 1), DirectX::XMFLOAT3(0,0,0));
	MaterialFactory::CreateDefaultMaterial(m_model, 3, 0, DirectX::XMFLOAT4(1.0f, 0, 0, 1), DirectX::XMFLOAT3(0,0,0));
}

void Enemy::OnPlayStart()
{
	if (!m_isActive)
	{
		return;
	}
	m_aiState.CallStateFunction(&EnemyAIState::OnPlayStart);
}