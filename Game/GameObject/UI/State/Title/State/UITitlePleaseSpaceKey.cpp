#include "UITitlePleaseSpaceKey.h"
#include <math.h>

GameUI::UITitle::UITitlePleaseSpaceKey::UITitlePleaseSpaceKey(UITitle* owner)
	:
	m_pleaseSpaceKey(nullptr),
	m_animTimer(0),
	m_owner(owner),
	m_startFlag(false)
{

}

GameUI::UITitle::UITitlePleaseSpaceKey::~UITitlePleaseSpaceKey()
{

}

void GameUI::UITitle::UITitlePleaseSpaceKey::Enter()
{
	m_animTimer = 0.0f;
	m_startFlag = false;
	if (!m_pleaseSpaceKey)
	{
		m_pleaseSpaceKey = new Crown::RenderObject::UiGraphic(L"Resource/Texture/UI/PleaseSpaceKey.png");
	}
	m_pleaseSpaceKey->SetPosition(DirectX::XMFLOAT2(0, UI_POSITION_Y));
	m_pleaseSpaceKey->SetSize(DirectX::XMFLOAT2(UI_SIZE, UI_SIZE));
}

void GameUI::UITitle::UITitlePleaseSpaceKey::Update(Timer& timer)
{
	m_animTimer += timer.GetSystemTime();
	if (m_startFlag)
	{
		m_pleaseSpaceKey->SetAlpha((std::sin(m_animTimer * BLINK_SPEED) + 1.0f) / 2);
		if (m_animTimer >= CHANGE_TIME)
		{
			m_owner->m_titleState.ChangeState(IState::Menu);
		}
	}
	else
	{
		m_pleaseSpaceKey->SetAlpha((std::sin(m_animTimer * FADE_SPEED) + 1.0f) / 2);
	}
}

void GameUI::UITitle::UITitlePleaseSpaceKey::Exit()
{
	delete m_pleaseSpaceKey;
	m_pleaseSpaceKey = nullptr;
}

void GameUI::UITitle::UITitlePleaseSpaceKey::InputSelect()
{
	m_startFlag = true;
	m_animTimer = 0.0f;
}
