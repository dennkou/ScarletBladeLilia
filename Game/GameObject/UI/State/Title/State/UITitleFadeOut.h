#pragma once
#include "./UITitleState.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"

class GameUI::UITitle::UITitleFadeOut : public GameUI::UITitle::UITitleState
{
public:
	UITitleFadeOut(UITitle* owner);
	virtual ~UITitleFadeOut() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;
private:
	static constexpr float CHANGE_TIME = 1000.0f;

	void SizeUpdate(Crown::RenderObject::UiGraphic* ui, bool isUpSize, float timer);

	UITitle* m_owner;
	float m_timer;
};