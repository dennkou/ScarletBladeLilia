#pragma once
#include "./../GameObject.h"

class GameUI : public Game::GameObject
{
public:
	GameUI(Game* game);
	virtual ~GameUI() override;

	virtual void OnGameInitialize() override;
	virtual void OnGameUpdate(Timer& timer) override;
	virtual void OnGameFinalize() override;

	virtual void OnInputSelect() override;
	virtual void OnInputCursorUp() override;
	virtual void OnInputCursorDown() override;
	virtual void OnInputCursorLeft() override;
	virtual void OnInputCursorRight() override;

	virtual void OnPlayRestart() override;

	virtual void OnPlayerDied() override;
private:
	enum class IState
	{
		Title,		//	タイトルUI
		Play,		//	プレイ中のUI
		Setting,	//	設定画面のUI
	};
	class UIState;
	class UITitle;
	class UIPlay;
	class UISetting;

	Crown::FiniteStateMachine<IState, UIState> m_state;
};