#pragma once
#include <vector>
#include <memory>
#include "./../GameObject.h"
#include "./StageObject/Sky.h"
#include "./StageObject/StageObject.h"

#include "./StageObject/Aisle.h"

//	ゲームオブジェクトを生成＆設定するクラスだよ☆
class Stage : public Game::GameObject
{
public:
	Stage(Game* game);
	virtual ~Stage() override;

	virtual void OnGameUpdate(Timer& timer) override;
private:
	Sky sky;
	std::vector<std::unique_ptr<StageObject>> m_stageObjects;
};