#pragma once
#include "./../GameObject.h"
#include "./Object/Object.h"

//	ゲームオブジェクトを生成＆設定するクラスだよ☆
class Stage : public Game::GameObject
{
public:
	Stage(Game* game);
	virtual ~Stage() override;
private:

};