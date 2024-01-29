#pragma once
#include "./../UITitle.h"



class GameUI::UITitle::UITitleState
{
public:
	UITitleState() {};
	virtual ~UITitleState() = default;

	virtual void Enter() = 0;
	virtual void Update(Timer& timer) = 0;
	virtual void Exit() = 0;

	virtual void InputMouse(DirectX::XMFLOAT2 input) { input; };
	virtual void InputCursorUp() {}
	virtual void InputCursorDown() {}
	virtual void InputCursorLeft() {}
	virtual void InputCursorRight() {}
	virtual void InputSelect() {}
private:

};