#pragma once

#include "./../UITitleMenu.h"

class GameUI::UITitle::UITitleMenu::TitleMenuState
{
public:
	TitleMenuState() = default;
	virtual ~TitleMenuState() = default;

	virtual void Enter() = 0;
	virtual void Update(Timer& timer) = 0;
	virtual void Exit() = 0;

	virtual void InputCursorUp() {};
	virtual void InputCursorDown() {};
	virtual void InputSelect() {};
};