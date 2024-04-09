#include "Enemy.h"
#include "State/EnemyState.h"
#include "State/EnemyStateTitle.h"
#include "State/Patrol/EnemyStatePatrol.h"
#include "State/Combat/EnemyStateCombat.h"

Enemy::Enemy(Game* game, DirectX::XMFLOAT3 positon, DirectX::XMFLOAT3 rotate, NavigationAI* navigationAI)
	:
	Character(game, positon, rotate, L"Resource/Model/PMX/enemy.pmx", MAX_HP),
	m_enemyCollider([&](int damage) { HitPlayerAttack(damage); })
{
	m_enemyCollider.SetRadius(2.0f);
	m_enemyCollider.SetCapsuleVector(DirectX::XMFLOAT3(0,20,0));
	m_enemyCollider.SetPosition(GetPosition());

	m_state.RegisterState<EnemyStateTitle>(State::Title, this);
	m_state.RegisterState<EnemyStatePatrol>(State::Patrol,this, navigationAI);
	m_state.RegisterState<EnemyStateCombat>(State::Combat,this);

	m_state.ChangeState(State::Title);

	m_ui.SetPositionOffset(UI_POSITION_OFFSET);
	m_ui.SetPosition(positon);
	m_ui.SetHPPercent(1);
}

Enemy::~Enemy()
{

}

void Enemy::OnGameUpdate(Timer& timer)
{
	timer;
	SetPosition(m_enemyCollider.GetPosition());

	m_state.CallFunction(&EnemyState::Update, timer.GetEnemyTime());

	m_enemyCollider.SetPosition(GetPosition());
	m_ui.SetPosition(GetPosition());
}

void Enemy::HitPlayerAttack(int damage)
{
	std::cout << "ìñÇΩÇ¡ÇΩÇÊÅô" << std::endl;
	Damage(damage);
	m_ui.SetHPPercent(static_cast<float>(GetHp()) / GetMaxHp());

	//	éÄñSèàóùÇæÇÊÅô
	if (!GetHp())
	{
		EventTrigger(&GameObject::OnEnemyDied, this);
		delete this;
	}
}

void Enemy::OnPlayStart()
{
	m_state.CallFunction(&EnemyState::OnPlayStart);
}