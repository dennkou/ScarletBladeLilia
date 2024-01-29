#pragma once
#include "TitleMenuState.h"
class GameUI::UITitle::UITitleMenu::TitleMenuTitleMove : public GameUI::UITitle::UITitleMenu::TitleMenuState
{
public:
	TitleMenuTitleMove(UITitleMenu* owner);
	virtual ~TitleMenuTitleMove();

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void InputSelect() override;
private:
	float m_timer;
	static constexpr float END_POSITION_X = 0.0f;		//	タイトルロゴの終了X座標だよ☆
	static constexpr float END_POSITION_Y = 0.5f;		//	タイトルロゴの終了Y座標だよ☆
	static constexpr float END_SIZE = 0.7f;				//	タイトルロゴの終了サイズだよ☆
	static constexpr float MOVE_TIME = 1000.0f;			//	繊維にかかる時間だよ☆

	UITitleMenu* m_owner;
};