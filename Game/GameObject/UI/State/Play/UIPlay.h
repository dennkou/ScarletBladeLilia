#pragma once
#include "./../UIState.h"
class GameUI::UIPlay : public GameUI::UIState
{
public:
	UIPlay();
	virtual ~UIPlay() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;
private:
};