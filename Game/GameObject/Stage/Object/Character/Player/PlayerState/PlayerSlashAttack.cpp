#include "PlayerSlashAttack.h"
#include "../../../MathLibrary.h"
#include <algorithm>

Player::PlayerSlashAttack::PlayerSlashAttack(Player* player)
	:
	m_player(player),
	m_animTimer(0.0f),
	m_attackCollider(20,
		{
			{ DirectX::XMFLOAT3(-3.255874f, 11.33067f, -6.845737f),	DirectX::XMFLOAT3(-8.377335f, 14.94956f, 0.2361572f),	DirectX::XMFLOAT3(-7.279235f, 15.83291f, -12.38734f) },
			{ DirectX::XMFLOAT3(-3.255874f, 11.33067f, -6.845737f),	DirectX::XMFLOAT3(-3.794989f, 13.03446f, -18.27532f),	DirectX::XMFLOAT3(-1.023521f, 7.688783f, -10.70738f) },
			{ DirectX::XMFLOAT3(-3.794989f, 13.03446f, -18.27532f),	DirectX::XMFLOAT3(-3.255874f, 11.33067f, -6.845737f),	DirectX::XMFLOAT3(-7.279235f, 15.83291f, -12.38734f) },
			{ DirectX::XMFLOAT3(-1.023521f, 7.688783f, -10.70738f),	DirectX::XMFLOAT3(-3.794989f, 13.03446f, -18.27532f),	DirectX::XMFLOAT3(1.1674f,	 4.716289f, -20.1535f) },
			{ DirectX::XMFLOAT3(-0.09770263f,4.525569f,-10.63023f),	DirectX::XMFLOAT3(-1.023521f, 7.688783f, -10.70738f),	DirectX::XMFLOAT3(1.1674f,	 4.716289f, -20.1535f) },
			{ DirectX::XMFLOAT3(-0.09770263f,4.525569f,-10.63023f),	DirectX::XMFLOAT3(4.289076f, 0.8479958f, -18.47116f),	DirectX::XMFLOAT3(0.566658f,  3.55046f,  -10.08374f) },
			{ DirectX::XMFLOAT3(-0.09770263f,4.525569f,-10.63023f),	DirectX::XMFLOAT3(1.1674f, 1.1674f, -20.1535f),	DirectX::XMFLOAT3(4.289076f, 0.8479958f, -18.47116f) },
		})
{

}

Player::PlayerSlashAttack::~PlayerSlashAttack()
{

}

void Player::PlayerSlashAttack::Enter()
{
	m_animTimer = 0.0f;
	m_player->m_model.Attack();
}

void Player::PlayerSlashAttack::Exit()
{
	m_player->m_camera.SetPosition(m_player->m_position); 
}

void Player::PlayerSlashAttack::Update(float time)
{
	//	フレームを進める
	m_animTimer += time * 1.5f;

	//	カメラ位置の更新☆
	DirectX::XMFLOAT3 cameraPosition = DirectX::XMFLOAT3(0,0,0);
	cameraPosition.x -= sin(m_player->m_rotate.y) * MOVING_DISTANCE * m_animTimer / MOVE_END;
	cameraPosition.z -= cos(m_player->m_rotate.y) * MOVING_DISTANCE * m_animTimer / MOVE_END;
	m_player->m_camera.SetPosition(VectorAdd(m_player->m_position, cameraPosition));

	//	アニメーション終了時デフォルトに戻るよ☆
	if (m_animTimer > END)
	{
		m_player->m_position = VectorAdd(m_player->m_position, cameraPosition);
		m_player->m_playerState.ChangeState(StateID::Stand);
	}

	//	攻撃判定の更新☆
	if (ATTACK_START_FLAME <= m_animTimer && m_animTimer <= ATTACK_END_FLAME)
	{
		DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(m_player->m_rotate.x, m_player->m_rotate.y, m_player->m_rotate.z) * DirectX::XMMatrixTranslation(m_player->m_position.x, m_player->m_position.y, m_player->m_position.z);
		m_attackCollider.SetPlayerWorld(matrix);
		m_attackCollider.SetActive(true);
	}
	else
	{
		m_attackCollider.SetActive(false);
	}

	//	回転の処理☆
	if (ROTATE_START_FLAME <= m_animTimer && m_animTimer <= ROTATE_END_FLAME)
	{

		float inputAngle = m_player->m_camera.GetRotate().y + atan2(-m_player->m_inputMove.x, -m_player->m_inputMove.y);
		m_player->m_rotate.y = std::clamp(inputAngle, m_player->m_rotate.y - ROTATE_SPEED, m_player->m_rotate.y + ROTATE_SPEED);
	}
}