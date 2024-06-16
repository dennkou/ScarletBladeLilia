#include "UITitle.h"

#include "./State/UITitleState.h"
#include "./State/UITitleFadeIn.h"
#include "./State/UITitlePleaseSpaceKey.h"
#include "./State/UITitleMenu.h"
#include "./State/UITitleFadeOut.h"

GameUI::UITitle::UITitle(GameUI* gameUI)
	:
	m_owner(gameUI),
	m_start(L"Resource/Texture/UI/GameStart.png"),
	m_end(L"Resource/Texture/UI/GameEnd.png"),
	m_option(L"Resource/Texture/UI/Option.png"),
	m_cursor(L"Resource/Texture/UI/Cursor.png"),
	m_titleLogo(L"Resource/Texture/UI/緋刃のリリアタイトルロゴ.png")
{
	//	ステートマシンの設定だよ☆
	m_titleState.SetEnterFunction(&UITitleState::Enter);
	m_titleState.SetExitFunction(&UITitleState::Exit);
	m_titleState.RegisterState<UITitleFadeIn>(IState::FadeIn, this);
	m_titleState.RegisterState<UITitlePleaseSpaceKey>(IState::PleaseSpaceKey, this);
	m_titleState.RegisterState<UITitleMenu>(IState::Menu, this);
	m_titleState.RegisterState<UITitleFadeOut>(IState::FadeOut, this);
	m_titleState.ChangeState(IState::FadeIn);

	//	uiの初期化だよ☆
	m_start.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_START_POS_Y));
	m_start.SetSize(DirectX::XMFLOAT2(SELECT_ITEM_SIZE_X, SELECT_ITEM_SIZE_Y));
	m_end.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_START_POS_Y));
	m_end.SetSize(DirectX::XMFLOAT2(SELECT_ITEM_SIZE_X, SELECT_ITEM_SIZE_Y));
	m_option.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_START_POS_Y));
	m_option.SetSize(DirectX::XMFLOAT2(SELECT_ITEM_SIZE_X, SELECT_ITEM_SIZE_Y));
	m_cursor.SetPosition(DirectX::XMFLOAT2(CURSOR_POSITION_X, CURSOR_START_POS_X));
	m_cursor.SetSize(DirectX::XMFLOAT2(CURSOR_SIZE_X, CURSOR_SIZE_Y));
}

GameUI::UITitle::~UITitle()
{

}

void GameUI::UITitle::Enter()
{
}

void GameUI::UITitle::Update(Timer& timer)
{
	m_titleState.CallStateFunction<void, Timer&>(&UITitleState::Update, timer);
}

void GameUI::UITitle::Exit()
{

}

void GameUI::UITitle::InputMouse(DirectX::XMFLOAT2 input)
{
	m_titleState.CallStateFunction(&UITitleState::InputMouse, input);
}

void GameUI::UITitle::InputCursorUp()
{
	m_titleState.CallStateFunction(&UITitleState::InputCursorUp);
}

void GameUI::UITitle::InputCursorDown()
{
	m_titleState.CallStateFunction(&UITitleState::InputCursorDown);
}

void GameUI::UITitle::InputSelect()
{
	m_titleState.CallStateFunction(&UITitleState::InputSelect);
}