#include "Player.h"
#include "./PlayerState/PlayerState.h"
#include "./PlayerState/PlayerTitle.h"
#include "./PlayerState/PlayerStateStand.h"
#include "./PlayerState/PlayerStateWalk.h"
#include "./PlayerState/PlayerStateRun.h"
#include "./PlayerState/PlayerSlashAttack.h"
#include "./PlayerState/PlayerAttack.h"
#include "./PlayerState/PlayerAvoidance.h"
#include "./PlayerState/PlayerDied.h"
#include <numbers>
#include <algorithm>



Player::Player(Game* game, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 rotate)
	:
	GameObject(game),
	m_model(),
	m_hp(MAX_HP),
	m_inputMove(0,0),
	m_velocity(0,0,0),
	m_collider([&](float damage) {Hit(damage); }),
	m_startPosition(position),
	m_startRotate(rotate),
	m_position(position),
	m_rotate(rotate)
{
	m_model.SetPosition(m_startPosition);
	m_model.SetRotate(m_startRotate);

	m_collider.SetPosition(m_startPosition);

	StateSetUp();
}

Player::~Player()
{

}

void Player::OnGameUpdate(Timer& timer)
{
	m_position = m_collider.GetPosition();
	m_playerState.CallStateFunction(&PlayerState::Update, timer.GetWorldTime());
	m_model.Update(timer.GetWorldTime());
	m_camera.UpdateCameraTransform();

	ColliderUpdate();
	m_model.SetPosition(m_position);
	m_model.SetRotate(m_rotate);
}

void Player::OnPlayStartAnimation()
{
	m_playerState.CallStateFunction(&PlayerState::OnPlayStartAnimation);
}

void Player::OnPlayStart()
{
	m_playerState.CallStateFunction(&PlayerState::OnPlayStart);
}

void Player::OnPlayRestart()
{
	m_model.SetPosition(m_startPosition);
	m_model.SetRotate(m_startRotate);
	m_collider.SetPosition(m_startPosition);
	m_playerState.ChangeState(StateID::Stand);
	m_hp.SetMaxHp(MAX_HP);
	m_hp.HpRecover(MAX_HP);
	m_hpUi.SetPlayerHPPercent(m_hp.GetHPPercent());
	m_camera.SetRotate(PlayerTitle::CAMERA_PLAY_ROTATE);
	m_camera.SetPosition(PlayerTitle::CAMERA_PLAY_POSITION);
}

void Player::OnInputMove(DirectX::XMFLOAT2 input)
{
	m_inputMove = input;
	m_playerState.CallStateFunction(&PlayerState::OnInputMove, input);
}

void Player::OnInputAttackDown()
{
	m_playerState.CallStateFunction(&PlayerState::OnInputAttackDown);
}

void Player::OnInputAvoidanceDown()
{
	m_playerState.CallStateFunction(&PlayerState::OnInputAvoidanceDown);
}

void Player::OnInputCamera(DirectX::XMFLOAT2 input)
{
	m_playerState.CallStateFunction(&PlayerState::OnInputCamera, input);
}

void Player::OnInputDash(bool input)
{
	m_playerState.CallStateFunction(&PlayerState::OnInputDash, input);
}

void Player::StateSetUp()
{
	m_playerState.RegisterState<PlayerTitle>(StateID::Title, this);
	m_playerState.RegisterState<PlayerStand>(StateID::Stand, this);
	m_playerState.RegisterState<PlayerWalk>(StateID::Walk, this);
	m_playerState.RegisterState<PlayerRun>(StateID::Run, this);
	m_playerState.RegisterState<PlayerAttack>(StateID::Attack, this);
	m_playerState.RegisterState<PlayerSlashAttack>(StateID::SlashAttack, this);
	m_playerState.RegisterState<PlayerAvoidance>(StateID::Avoidance, this);
	m_playerState.RegisterState<PlayerDied>(StateID::Died, this);

	m_playerState.SetEnterFunction(&PlayerState::Enter);
	m_playerState.SetExitFunction(&PlayerState::Exit);

	m_playerState.ChangeState(StateID::Title);
}

void Player::ColliderUpdate()
{
	m_collider.SetPosition(m_position);

	DirectX::XMMATRIX head = m_model.GetBone(L"head");
	DirectX::XMVECTOR scale, rotate, transform;
	DirectX::XMMatrixDecompose(&scale, &rotate, &transform, head);
	DirectX::XMFLOAT3 position;
	transform = DirectX::XMVectorAdd(transform, DirectX::XMVECTOR({ 0.0f, 15.0f, 0.0f, 0.0f }));
	DirectX::XMStoreFloat3(&position, transform);
	m_collider.SetCapsuleVector(position);
}

void Player::CameraRoll(DirectX::XMFLOAT2 input)
{
	DirectX::XMFLOAT3 cameraRotate = m_camera.GetRotate();
	cameraRotate.y = fmodf(cameraRotate.y + input.x / 1000.0f, static_cast<float>(std::numbers::pi * 2));

	cameraRotate.x = fmodf(cameraRotate.x + input.y / 1000.0f, static_cast<float>(std::numbers::pi * 2));
	cameraRotate.x = std::clamp(cameraRotate.x, static_cast<float>(-std::numbers::pi / 2), static_cast<float>(std::numbers::pi / 2));
	m_camera.SetRotate(cameraRotate);
}

void Player::Hit(float damage)
{
	m_hp.Damage(damage);

	m_hpUi.SetPlayerHPPercent(m_hp.GetHPPercent());

	if (m_hp.IsDied())
	{
		m_playerState.ChangeState(StateID::Died);
	}
}