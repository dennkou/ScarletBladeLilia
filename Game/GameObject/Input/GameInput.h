#pragma once
#include "./../GameObject.h"
class GameInput : public Game::GameObject
{
public:
	GameInput(Game* game);
	virtual ~GameInput();

	virtual void OnGameUpdate(Timer& timer) override;

	virtual void OnInputMenu() override;

	virtual void OnPlayStart() override;
private:
	DirectX::XMFLOAT2 m_input;
	DirectX::XMFLOAT2 m_mouse;
	bool m_mouseMove;
};