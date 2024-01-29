#pragma once
#include "./UITitleState.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"

class GameUI::UITitle::UITitlePleaseSpaceKey : public GameUI::UITitle::UITitleState
{
public:
	UITitlePleaseSpaceKey(UITitle* owner);
	virtual ~UITitlePleaseSpaceKey() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void InputSelect() override;
private:
	static constexpr float UI_POSITION_Y = -0.45f;
	static constexpr float UI_SIZE = 0.2f;

	static constexpr float FADE_SPEED = 0.002f;
	static constexpr float BLINK_SPEED = 0.05f;
	static constexpr float CHANGE_TIME = 500.0f;

	UITitle* m_owner;
	Crown::RenderObject::UiGraphic* m_pleaseSpaceKey;
	float m_timer;
	bool m_startFlag;
};

