#include "GameUI.h"
#include "./State/UIState.h"
#include "./State/Title/UITitle.h"
#include "./State/Play/UIPlay.h"

GameUI::GameUI(Game* game)
	:
	GameObject(game)
{
	//	ステートマシンの設定だよ☆
	m_state.SetEnterFunction(&UIState::Enter);
	m_state.SetExitFunction(&UIState::Exit);
	m_state.RegisterState<UITitle>(State::Title, this);
	m_state.RegisterState<UIPlay>(State::Play);
	m_state.ChangeState(State::Title);
}

GameUI::~GameUI()
{
}

void GameUI::OnGameInitialize()
{
}

void GameUI::OnGameUpdate(Timer& timer)
{
	m_state.CallStateFunction<void, Timer&>(&UIState::Update, timer);
}

void GameUI::OnGameFinalize()
{
}

void GameUI::OnInputSelect()
{
	m_state.CallStateFunction(&UIState::InputSelect);
}

void GameUI::OnInputCursorUp()
{
	m_state.CallStateFunction(&UIState::InputCursorUp);
}

void GameUI::OnInputCursorDown()
{
	m_state.CallStateFunction(&UIState::InputCursorDown);
}

void GameUI::OnInputCursorLeft()
{
	m_state.CallStateFunction(&UIState::InputCursorLeft);
}

void GameUI::OnInputCursorRight()
{
	m_state.CallStateFunction(&UIState::InputCursorRight);
}

void GameUI::OnPlayerDied()
{
	m_state.CallStateFunction(&UIState::OnPlayerDied);
}