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
	static constexpr float END_POSITION_X = 0.0f;		//	�^�C�g�����S�̏I��X���W���恙
	static constexpr float END_POSITION_Y = 0.5f;		//	�^�C�g�����S�̏I��Y���W���恙
	static constexpr float END_SIZE = 0.7f;				//	�^�C�g�����S�̏I���T�C�Y���恙
	static constexpr float MOVE_TIME = 1000.0f;			//	�@�ۂɂ����鎞�Ԃ��恙

	UITitleMenu* m_owner;
};