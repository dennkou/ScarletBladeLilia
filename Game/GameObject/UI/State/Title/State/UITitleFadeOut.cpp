#include "UITitleFadeOut.h"

GameUI::UITitle::UITitleFadeOut::UITitleFadeOut(UITitle* owner)
	:
	m_owner(owner),
	m_animTimer(0)
{

}

GameUI::UITitle::UITitleFadeOut::~UITitleFadeOut()
{

}

void GameUI::UITitle::UITitleFadeOut::Enter()
{
	m_owner->m_owner->EventTrigger(&GameObject::OnPlayStartAnimation);
}

void GameUI::UITitle::UITitleFadeOut::Update(Timer& timer)
{
	m_animTimer += timer.GetSystemTime();

	m_owner->m_titleLogo.SetAlpha(1 - (m_animTimer / CHANGE_TIME));
	m_owner->m_start.SetAlpha(1 - (m_animTimer / CHANGE_TIME));
	m_owner->m_end.SetAlpha(1 - (m_animTimer / CHANGE_TIME));
	m_owner->m_option.SetAlpha(1 - (m_animTimer / CHANGE_TIME));
	m_owner->m_cursor.SetAlpha(1 - (m_animTimer / CHANGE_TIME));

	if (m_animTimer >= CHANGE_TIME)
	{
		m_owner->m_owner->m_state.ChangeState(GameUI::State::Play);

		//	このステートを削除するよ☆
		m_owner->m_owner->m_state.DeleteState(GameUI::State::Title);
		return;
	}

	//	カーソルの動きが不自然にならないようにメニューの動きを再現するよ☆
	float posY = m_owner->m_cursor.GetPosition().y;
	posY = std::lerp(posY, SELECT_ITEM_POSITION_Y, timer.GetSystemTime() * CURSOR_SPEED);
	m_owner->m_cursor.SetPosition(DirectX::XMFLOAT2(CURSOR_POSITION_X, posY));
	SizeUpdate(&m_owner->m_start, true, timer.GetSystemTime());
	SizeUpdate(&m_owner->m_end, false, timer.GetSystemTime());
	SizeUpdate(&m_owner->m_option, false, timer.GetSystemTime());
}

void GameUI::UITitle::UITitleFadeOut::Exit()
{

}


void GameUI::UITitle::UITitleFadeOut::SizeUpdate(Crown::RenderObject::UiGraphic* ui, bool isUpSize, float timer)
{
	auto SizeDown = [&](float min, float target)
		{
			if (target < min || timer * SIZE_DOWN_SPEED > 1.0f)
			{
				return min;
			}
			return std::lerp(target, min, timer * SIZE_DOWN_SPEED);
		};

	auto SizeUp = [&](float max, float target)
		{
			if (target > max || timer * SIZE_UP_SPEED > 1.0f)
			{
				return max;
			}
			return std::lerp(target, max, timer * SIZE_UP_SPEED);
		};

	DirectX::XMFLOAT2 size = ui->GetSize();

	if (isUpSize)
	{
		size.x = SizeUp(SELECT_SIZE_X, size.x);
		size.y = SizeUp(SELECT_SIZE_Y, size.y);
	}
	else
	{
		size.x = SizeDown(SELECT_ITEM_SIZE_X, size.x);
		size.y = SizeDown(SELECT_ITEM_SIZE_Y, size.y);
	}

	ui->SetSize(size);
}