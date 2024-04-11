#pragma once
#include "./../GameObject.h"
#include "./Object/Object.h"
#include "./StageObject/Sky.h"

//	�Q�[���I�u�W�F�N�g�𐶐����ݒ肷��N���X���恙
class Stage : public Game::GameObject
{
public:
	Stage(Game* game);
	virtual ~Stage() override;

	virtual void OnGameUpdate(Timer& timer) override;
private:
	Sky sky;
};