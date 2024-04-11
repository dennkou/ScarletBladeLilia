#pragma once
#include "./../GameObject.h"
#include "./Object/Object.h"
#include "./StageObject/Sky.h"

//	ゲームオブジェクトを生成＆設定するクラスだよ☆
class Stage : public Game::GameObject
{
public:
	Stage(Game* game);
	virtual ~Stage() override;

	virtual void OnGameUpdate(Timer& timer) override;
private:
	Sky sky;
};