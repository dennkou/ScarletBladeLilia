#include "Enemy.h"
#include "AIState/EnemyAIState.h"
#include "AIState/EnemyAIStateTitle.h"
#include "AIState/EnemyAIStatePatrol.h"
#include "AIState/EnemyAIStateCombat.h"

#include "./../../../../../../MathLibrary.h"

Enemy::Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate, NavigationAI* navigationAI, std::shared_ptr<std::mt19937> random)
	:
	GameObject(game),
	m_position(positon),
	m_rotate(rotate),
	m_hp(MAX_HP),
	m_enemyCollider([&](float damage) { HitPlayerAttack(damage); }, [&]() { HitWall(); }),
	m_offset(),
	m_random(random),
	m_damageAnimTimer(DamageAnimLength),
	m_damageAnim()
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
}

Enemy::~Enemy()
{

}

void Enemy::OnGameUpdate(Timer& timer)
{
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

	m_position = VectorSub(m_enemyCollider.GetPosition(), m_offset);

	m_aiState.CallStateFunction(&EnemyAIState::Update, timer.GetEnemyTime());

	if (m_position.y > 0)
	{
		m_position.y -= 0.1f * timer.GetEnemyTime();
	}

	m_model.SetPause(m_bone);

	//	当たり判定の位置を設定するよ☆
	m_model.GetBoneDate().Find(L"全ての親").RecursiveMatrixMultiply(m_bone);
	unsigned int index = m_model.GetBoneDate().Find(L"本体").GetIndex();
	m_offset = VectorAdd(DirectX::XMFLOAT3(m_bone[index].r[3].m128_f32[0], m_bone[index].r[3].m128_f32[1], m_bone[index].r[3].m128_f32[2]), BONE_OFFSET);

	m_enemyCollider.SetPosition(VectorAdd(m_offset, m_position));
	m_ui.SetPosition(m_position);
	m_model.SetPosition(VectorAdd(m_position, m_damageAnim));
	m_model.SetRotate(m_rotate);
}

void Enemy::HitPlayerAttack(float damage)
{
	m_damageAnimTimer = 0.0f;
	m_hp.Damage(damage);
	m_aiState.CallStateFunction(&EnemyAIState::OnDamage, damage);
	m_ui.SetHPPercent(m_hp.GetHPPercent());

	//	死亡処理だよ☆
	if (m_hp.IsDied())
	{
		EventTrigger(&GameObject::OnEnemyDied, this);
		delete this;
	}
}

void Enemy::HitWall()
{
	m_aiState.CallStateFunction(&EnemyAIState::OnWallHit);
}

void Enemy::OnPlayStart()
{
	m_aiState.CallStateFunction(&EnemyAIState::OnPlayStart);
}