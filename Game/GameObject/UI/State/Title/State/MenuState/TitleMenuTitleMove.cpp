#include "TitleMenuTitleMove.h"

GameUI::UITitle::UITitleMenu::TitleMenuTitleMove::TitleMenuTitleMove(UITitleMenu* owner)
	:
	m_owner(owner)
{

}

GameUI::UITitle::UITitleMenu::TitleMenuTitleMove::~TitleMenuTitleMove()
{

}

void GameUI::UITitle::UITitleMenu::TitleMenuTitleMove::Enter()
{
	m_owner->m_owner->m_titleLogo.SetSize(DirectX::XMFLOAT2(1.0f,1.0f));
}

void GameUI::UITitle::UITitleMenu::TitleMenuTitleMove::Update(Timer& timer)
{
	m_animTimer += timer.GetSystemTime() / MOVE_TIME;
	float t = sin(m_animTimer * DirectX::XM_PI / 2);

	m_owner->m_owner->m_titleLogo.SetPosition(DirectX::XMFLOAT2(END_POSITION_X * t, END_POSITION_Y * t));

	float size = std::lerp(1.0f, END_SIZE, t);
	m_owner->m_owner->m_titleLogo.SetSize(DirectX::XMFLOAT2(size, size));

	if (t >= 0.999f)
	{
		m_owner->m_state.ChangeState(IState::MenuIn);
	}
}

void GameUI::UITitle::UITitleMenu::TitleMenuTitleMove::Exit()
{
	m_owner->m_owner->m_titleLogo.SetSize(DirectX::XMFLOAT2(END_SIZE, END_SIZE));
	m_owner->m_owner->m_titleLogo.SetPosition(DirectX::XMFLOAT2(END_POSITION_X, END_POSITION_Y));
}

void GameUI::UITitle::UITitleMenu::TitleMenuTitleMove::InputSelect()
{
	m_owner->m_state.ChangeState(IState::Menu);
}