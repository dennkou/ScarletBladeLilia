#pragma once
#include "./../GameObject.h"
class Render : public Game::GameObject
{
public:
	Render(Game* game);
	virtual ~Render() override;

	void OnGameUpdate(Timer& timer) override;
private:
};