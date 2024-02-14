#pragma once
#include "./../UIState.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"

class GameUI::UIPlay : public GameUI::UIState
{
public:
	UIPlay();
	virtual ~UIPlay() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;
private:
	Crown::RenderObject::UiGraphic m_playerUi;
};