#pragma once
#include "./UITitleState.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"

class GameUI::UITitle::UITitleMenu : public GameUI::UITitle::UITitleState
{
public:
	UITitleMenu(UITitle* uiOutputTitle);
	virtual ~UITitleMenu() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void InputCursorUp() override;
	virtual void InputCursorDown() override;
	virtual void InputSelect() override;
private:
	UITitle* m_owner;

	enum class State
	{
		TitleMove,
		MenuIn,
		Menu,
		Select
	};

	class TitleMenuState;
	class TitleMenuTitleMove;
	class TitleMenuMenuIn;
	class TitleMenu;
	class TitleMenuSelect;

	enum class SelectItem
	{
		Start = 0,
		End = 1,
		Option = 2,

		NUM,
	};
	SelectItem m_selectItem;

	Crown::FiniteStateMachine<State, TitleMenuState> m_state;
};