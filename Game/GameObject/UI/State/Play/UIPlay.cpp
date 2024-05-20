#include "UIPlay.h"

GameUI::UIPlay::UIPlay(GameUI* owner)
	:
	m_owner(owner)
{
	m_state = State::Play;
	m_alpha = 0.0f;

	m_gameClear.Load(L"Resource/Texture/UI/GAME CLEAR.png");
	m_gameOver.Load(L"Resource/Texture/UI/GAMEOVER.png");
	m_fade.Load(L"黒テクスチャ");
	m_gameClear.SetAlpha(m_alpha);
	m_gameOver.SetAlpha(m_alpha);
	m_fade.SetAlpha(m_alpha);

	m_fade.SetPriority(-1);
	m_gameClear.SetPriority(1);
	m_gameOver.SetPriority(1);
}

GameUI::UIPlay::~UIPlay()
{

}

void GameUI::UIPlay::Enter()
{
}

void GameUI::UIPlay::Update(Timer& timer)
{
	timer;

	switch (m_state)
	{
	case GameUI::UIPlay::State::Play:

		break;
	case GameUI::UIPlay::State::GameOver:
		if (m_alpha < 1.0f)
		{
			m_alpha += timer.GetSystemTime() * ALPHA_SPEED;
		}
		m_gameOver.SetAlpha(m_alpha);
		m_fade.SetAlpha(m_alpha);
		break;
	case GameUI::UIPlay::State::GameClear:
		if (m_alpha < 1.0f)
		{
			m_alpha += timer.GetSystemTime() * ALPHA_SPEED;
		}
		m_gameClear.SetAlpha(m_alpha);
		m_fade.SetAlpha(m_alpha);
		break;
	}
}

void GameUI::UIPlay::Exit()
{

}

void GameUI::UIPlay::OnPlayerDied()
{
	m_state = State::GameOver;
}

void GameUI::UIPlay::InputSelect()
{
	switch (m_state)
	{
	case GameUI::UIPlay::State::Play:

		break;
	case GameUI::UIPlay::State::GameOver:
		if (m_alpha >= 1.0f)
		{
			m_owner->EventTrigger(&Game::GameObject::OnPlayRestart);
		}
		break;
	case GameUI::UIPlay::State::GameClear:

		break;
	}
}

void GameUI::UIPlay::OnPlayRestart()
{
	m_alpha = 0.0f;
	m_state = State::Play;
	m_gameClear.SetAlpha(m_alpha);
	m_gameOver.SetAlpha(m_alpha);
	m_fade.SetAlpha(m_alpha);
}