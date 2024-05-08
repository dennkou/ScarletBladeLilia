#pragma once
#include "./../GameObject.h"
#include "./../../Crown/Object/RenderSystem/RenderTarget.h"
class Render : public Game::GameObject
{
public:
	Render(Game* game);
	virtual ~Render() override;

	void OnGameUpdate(Timer& timer) override;
private:

};