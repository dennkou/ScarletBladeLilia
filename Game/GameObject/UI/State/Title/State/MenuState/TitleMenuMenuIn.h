#pragma once
#include "./TitleMenuState.h"

class GameUI::UITitle::UITitleMenu::TitleMenuMenuIn : public GameUI::UITitle::UITitleMenu::TitleMenuState
{
public:
	TitleMenuMenuIn(UITitleMenu* owner);
	virtual ~TitleMenuMenuIn() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void InputSelect() {};
private:
	float m_timer;
	UITitleMenu* m_owner;
};