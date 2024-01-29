#include "UITitleFadeIn.h"

GameUI::UITitle::UITitleFadeIn::UITitleFadeIn(UITitle* uiOutputTitle)
	:
	m_owner(uiOutputTitle),
	m_fade(nullptr),
	m_fadeAlpha(1.0f)
{

}

GameUI::UITitle::UITitleFadeIn::~UITitleFadeIn()
{
	if (m_fade)
	{
		delete m_fade;
	}
}

void GameUI::UITitle::UITitleFadeIn::Enter()
{
	m_fadeAlpha = 1.0f;
	m_fade = new Crown::RenderObject::UiGraphic(L"黒テクスチャ");
	m_fade->SetAlpha(m_fadeAlpha);
	m_fade->SetPriority(2147483647);
	m_owner->m_titleLogo.SetAlpha(1.0f);
}

void GameUI::UITitle::UITitleFadeIn::Update(Timer& timer)
{
	m_fadeAlpha -= timer.GetTime() * FADE_SPEED;
	m_fade->SetAlpha(m_fadeAlpha);
	if (m_fadeAlpha <= 0.0f)
	{
		m_owner->m_titleState.ChangeState(State::PleaseSpaceKey);
	}
}

void GameUI::UITitle::UITitleFadeIn::Exit()
{
	m_fadeAlpha = 1.0f;
	delete m_fade;
	m_fade = nullptr;
}

void GameUI::UITitle::UITitleFadeIn::InputSelect()
{
	m_owner->m_titleState.ChangeState(State::PleaseSpaceKey);
}