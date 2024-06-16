#include "PlayerSlashAttack.h"
#include "../../../MathLibrary.h"

Player::PlayerSlashAttack::PlayerSlashAttack(Player* player)
	:
	m_player(player),
	m_animTimer(0.0f)
{

}

Player::PlayerSlashAttack::~PlayerSlashAttack()
{

}

void Player::PlayerSlashAttack::Enter()
{
	m_animTimer = 0.0f;
}

void Player::PlayerSlashAttack::Exit()
{

}

void Player::PlayerSlashAttack::Update(float time)
{
	m_animTimer += time * 0.5f;
	float animFlame = m_animTimer / ANIMATION_FPS;
	m_player->m_slashAttackAnim.GetAnimation(animFlame, m_player->m_bone, m_player->m_model.GetBoneDate());

	//	アニメーション終了時デフォルトに戻るよ☆
	if (animFlame > m_player->m_slashAttackAnim.GetMaxFrame())
	{
		m_player->m_camera.SetPosition(m_player->m_position);
		m_player->m_position.x -= sin(m_player->m_rotate.y) * MOVING_DISTANCE * 20;
		m_player->m_position.z -= cos(m_player->m_rotate.y) * MOVING_DISTANCE * 20;
		m_player->m_playerState.ChangeState(StateID::Stand);
	}

	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0,0,0);
	cameraPosition.x -= sin(m_player->m_rotate.y) * MOVING_DISTANCE * animFlame / m_player->m_slashAttackAnim.GetMaxFrame();
	cameraPosition.z -= cos(m_player->m_rotate.y) * MOVING_DISTANCE * animFlame / m_player->m_slashAttackAnim.GetMaxFrame();

	m_player->m_camera.SetPosition(VectorAdd(m_player->m_position, cameraPosition));
}