#pragma once
#include "./TitleMenuState.h"

class GameUI::UITitle::UITitleMenu::TitleMenu : public GameUI::UITitle::UITitleMenu::TitleMenuState
{
public:
	TitleMenu(UITitleMenu* owner);
	virtual ~TitleMenu() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void InputCursorUp();
	virtual void InputCursorDown();
	virtual void InputSelect();

private:
	void SelectionUpdate(bool isUp);
	void SizeUpdate(Crown::RenderObject::UiGraphic* ui, bool isUpSize, float timer);

	UITitleMenu* m_owner;
	float m_targetPositionY;
	float m_timer;
};