#pragma once
#include "./../Player.h"

class Player::PlayerState
{
public:
	PlayerState() {};
	virtual ~PlayerState() {};

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Update(float time) = 0;

	//	���͌n��
	virtual void OnInputMove(DirectX::XMFLOAT2 input) { input; }	//	�ړ��̓��͑��삪�������恙
	virtual void OnInputAttackDown() {}								//	�U���{�^���������ꂽ�恙
	virtual void OnInputAvoidanceDown() {}							//	����{�^���������ꂽ�恙
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) { input; }	//	�J�����̓��͂��������恙
	virtual void OnPlayStartAnimation() {};							//	�v���C�J�n���I�����ꂽ�恙
	virtual void OnPlayStart() {};									//	�Q�[���v���C���J�n���ꂽ�恙

private:
};