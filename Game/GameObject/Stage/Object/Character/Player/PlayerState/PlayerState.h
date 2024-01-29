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

	//	入力系☆
	virtual void OnInputMove(DirectX::XMFLOAT2 input) { input; }	//	移動の入力操作があったよ☆
	virtual void OnInputAttackDown() {}								//	攻撃ボタンが押されたよ☆
	virtual void OnInputAvoidanceDown() {}							//	回避ボタンが押されたよ☆
	virtual void OnInputCamera(DirectX::XMFLOAT2 input) { input; }	//	カメラの入力があったよ☆
	virtual void OnPlayStartAnimation() {};							//	プレイ開始が選択されたよ☆
	virtual void OnPlayStart() {};									//	ゲームプレイが開始されたよ☆

private:
};