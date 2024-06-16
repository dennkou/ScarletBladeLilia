#include "TitleMenuMenuIn.h"
#include <algorithm>

GameUI::UITitle::UITitleMenu::TitleMenuMenuIn::TitleMenuMenuIn(UITitleMenu* owner)
	:
	m_owner(owner)
{

}

GameUI::UITitle::UITitleMenu::TitleMenuMenuIn::~TitleMenuMenuIn()
{

}

void GameUI::UITitle::UITitleMenu::TitleMenuMenuIn::Enter()
{
	m_owner->m_owner->m_start.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_START_POS_Y));
	m_owner->m_owner->m_end.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_START_POS_Y));
	m_owner->m_owner->m_option.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_START_POS_Y));
	m_animTimer = 0;
}

void GameUI::UITitle::UITitleMenu::TitleMenuMenuIn::Update(Timer& timer)
{
	auto PosUpdate = [](float startPos, float endPos, float t) -> float
	{
		t = std::sin(std::clamp(t, 0.0f, 1.0f) * DirectX::XM_PI / 2);
		return std::lerp(startPos, endPos, t);
	};

	m_animTimer += timer.GetSystemTime() * SELECT_ITEM_SPEED;

	m_owner->m_owner->m_start.SetPosition(DirectX::XMFLOAT2(0, PosUpdate(SELECT_ITEM_START_POS_Y, SELECT_ITEM_POSITION_Y, m_animTimer)));
	if (m_animTimer >= SELECT_ITEM_INTERVAL)
	{
		m_owner->m_owner->m_end.SetPosition(DirectX::XMFLOAT2(0, PosUpdate(SELECT_ITEM_START_POS_Y, SELECT_ITEM_POSITION_Y - SELECT_ITEM_POSITION_GAP_Y, m_animTimer - SELECT_ITEM_INTERVAL)));
	}
	if (m_animTimer >= (SELECT_ITEM_INTERVAL * 2))
	{
		m_owner->m_owner->m_option.SetPosition(DirectX::XMFLOAT2(0, PosUpdate(SELECT_ITEM_START_POS_Y, SELECT_ITEM_POSITION_Y - (SELECT_ITEM_POSITION_GAP_Y * 2), m_animTimer - (SELECT_ITEM_INTERVAL * 2))));
	}
	if (m_animTimer >= (SELECT_ITEM_INTERVAL * 3))
	{
		m_owner->m_owner->m_cursor.SetPosition(DirectX::XMFLOAT2(PosUpdate(CURSOR_START_POS_X, CURSOR_POSITION_X, m_animTimer - (SELECT_ITEM_INTERVAL * 3)), SELECT_ITEM_POSITION_Y));
	}

	if ((m_animTimer - (SELECT_ITEM_INTERVAL * 3)) >= 1.0f)
	{
		m_owner->m_state.ChangeState(IState::Menu);
	}
}

void GameUI::UITitle::UITitleMenu::TitleMenuMenuIn::Exit()
{

}