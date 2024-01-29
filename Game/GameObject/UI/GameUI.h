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
private:
	enum class State
	{
		Title,		//	�^�C�g��UI
		Play,		//	�v���C����UI
		Setting,	//	�ݒ��ʂ�UI
	};
	class UIState;
	class UITitle;
	class UIPlay;
	class UISetting;

	Crown::FiniteStateMachine<State, UIState> m_state;
};