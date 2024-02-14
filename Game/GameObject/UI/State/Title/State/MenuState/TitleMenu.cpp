#include "TitleMenu.h"

GameUI::UITitle::UITitleMenu::TitleMenu::TitleMenu(UITitleMenu* owner)
	:
	m_owner(owner),
	m_targetPositionY(SELECT_ITEM_POSITION_Y),
	m_timer(0)
{

}

GameUI::UITitle::UITitleMenu::TitleMenu::~TitleMenu()
{

}

void GameUI::UITitle::UITitleMenu::TitleMenu::TitleMenu::Enter()
{
	m_timer = 0;
	m_owner->m_owner->m_start.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_POSITION_Y));
	m_owner->m_owner->m_end.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_POSITION_Y - SELECT_ITEM_POSITION_GAP_Y));
	m_owner->m_owner->m_option.SetPosition(DirectX::XMFLOAT2(0, SELECT_ITEM_POSITION_Y - (SELECT_ITEM_POSITION_GAP_Y * 2)));
	m_owner->m_owner->m_cursor.SetPosition(DirectX::XMFLOAT2(CURSOR_POSITION_X, SELECT_ITEM_POSITION_Y));
}

void GameUI::UITitle::UITitleMenu::TitleMenu::Update(Timer& timer)
{
	m_timer += timer.GetSystemTime();

	float posY = m_owner->m_owner->m_cursor.GetPosition().y;

	posY = std::lerp(posY, m_targetPositionY, timer.GetSystemTime() * CURSOR_SPEED);

	m_owner->m_owner->m_cursor.SetPosition(DirectX::XMFLOAT2(CURSOR_POSITION_X, posY));

	SizeUpdate(&m_owner->m_owner->m_start, m_owner->m_selectItem == SelectItem::Start, timer.GetSystemTime());
	SizeUpdate(&m_owner->m_owner->m_end, m_owner->m_selectItem == SelectItem::End, timer.GetSystemTime());
	SizeUpdate(&m_owner->m_owner->m_option, m_owner->m_selectItem == SelectItem::Option, timer.GetSystemTime());
}

void GameUI::UITitle::UITitleMenu::TitleMenu::Exit()
{
}

//	各イベントのハンドラーだよ☆

void GameUI::UITitle::UITitleMenu::TitleMenu::InputCursorUp()
{
	SelectionUpdate(true);
}

void GameUI::UITitle::UITitleMenu::TitleMenu::InputCursorDown()
{
	SelectionUpdate(false);
}

void GameUI::UITitle::UITitleMenu::TitleMenu::InputSelect()
{
	switch (m_owner->m_selectItem)
	{
		case SelectItem::Start:
		{
			m_owner->m_owner->m_titleState.ChangeState(GameUI::UITitle::State::FadeOut);
		}
		break;
		case SelectItem::End:
		{
			m_owner->m_owner->m_owner->GameEnd();
		}
		break;
		case SelectItem::Option:
		{

		}
		break;
	}
}

//	内部で使用する関数☆

void GameUI::UITitle::UITitleMenu::TitleMenu::SelectionUpdate(bool isUp)
{
	int select = 0;
	if (isUp)
	{
		select = (static_cast<int>(m_owner->m_selectItem) - 1) % static_cast<int>(SelectItem::NUM);
	}
	else
	{
		select = (static_cast<int>(m_owner->m_selectItem) + 1) % static_cast<int>(SelectItem::NUM);
	}
	if (select < 0)
	{
		select = static_cast<int>(SelectItem::NUM) - 1;
	}
	m_owner->m_selectItem = static_cast<SelectItem>(select);

	m_targetPositionY = SELECT_ITEM_POSITION_Y - (SELECT_ITEM_POSITION_GAP_Y * select);
}

void GameUI::UITitle::UITitleMenu::TitleMenu::SizeUpdate(Crown::RenderObject::UiGraphic* ui, bool isUpSize, float timer)
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