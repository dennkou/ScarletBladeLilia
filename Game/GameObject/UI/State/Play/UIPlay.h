#pragma once
#include "./../UIState.h"
#include "./../../Crown/Object/RenderSystem/UI/UIType/UiGraphic.h"

class GameUI::UIPlay : public GameUI::UIState
{
public:
	UIPlay(GameUI* owner);
	virtual ~UIPlay() override;

	virtual void Enter() override;
	virtual void Update(Timer& timer) override;
	virtual void Exit() override;

	virtual void OnPlayerDied() override;
	virtual void InputSelect() override;
	virtual void OnPlayRestart() override;
private:
	enum class State
	{
		Play,
		GameOver,
		GameClear,
	};

	GameUI* m_owner;

	float m_alpha;
	static constexpr float ALPHA_SPEED = 0.0005f;

	State m_state;

	Crown::RenderObject::UiGraphic m_gameClear;
	Crown::RenderObject::UiGraphic m_gameOver;
	Crown::RenderObject::UiGraphic m_fade;
};