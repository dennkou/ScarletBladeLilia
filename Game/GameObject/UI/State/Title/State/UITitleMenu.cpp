#include "UITitleMenu.h"

#include "./MenuState/TitleMenuState.h"
#include "./MenuState/TitleMenuTitleMove.h"
#include "./MenuState/TitleMenuMenuIn.h"
#include "./MenuState/TitleMenu.h"

GameUI::UITitle::UITitleMenu::UITitleMenu(UITitle* uiOutputTitle)
	:
	m_owner(uiOutputTitle),
	m_selectItem(SelectItem::Start)
{
	m_state.SetEnterFunction(&TitleMenuState::Enter);
	m_state.SetExitFunction(&TitleMenuState::Exit);
	m_state.RegisterState<TitleMenuTitleMove>(State::TitleMove, this);
	m_state.RegisterState<TitleMenuMenuIn>(State::MenuIn, this);
	m_state.RegisterState<TitleMenu>(State::Menu, this);

	m_state.ChangeState(State::TitleMove);
}

GameUI::UITitle::UITitleMenu::~UITitleMenu()
{

}

void GameUI::UITitle::UITitleMenu::Enter()
{
}

void GameUI::UITitle::UITitleMenu::Update(Timer& timer)
{
	m_state.CallFunction<void, Timer&>(&TitleMenuState::Update, timer);
}

void GameUI::UITitle::UITitleMenu::Exit()
{
}

void GameUI::UITitle::UITitleMenu::InputCursorUp()
{
	m_state.CallFunction(&TitleMenuState::InputCursorUp);
}

void GameUI::UITitle::UITitleMenu::InputCursorDown()
{
	m_state.CallFunction(&TitleMenuState::InputCursorDown);
}

void GameUI::UITitle::UITitleMenu::InputSelect()
{
	m_state.CallFunction(&TitleMenuState::InputSelect);
}